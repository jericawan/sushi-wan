#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <cstdio>
#include <csignal>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "Sushi.hh"
// got help from qian qian, damir, chatgpt, and friend from outside school

Sushi::Sushi()
{
  prevent_interruption();

  const char *home_dir = std::getenv("HOME");

  // OK if missing!
  if (home_dir)
  {
    const std::string config_path = std::string(home_dir) + "/" + DEFAULT_CONFIG;
    read_config(config_path.c_str(), true);
  }
}

std::string Sushi::read_line(std::istream &in)
{
  std::string line;
  if (!std::getline(in, line))
  { // Has the operation failed?
    if (!in.eof())
    {
      std::perror("getline");
    }
    return "";
  }

  // Is the line empty?
  if (std::all_of(line.begin(), line.end(), isspace))
  {
    return "";
  }

  // Is the line too long?
  if (line.size() > MAX_INPUT_SIZE)
  {
    line.resize(MAX_INPUT_SIZE);
    std::cerr << "Line too long, truncated." << std::endl;
  }

  return line;
}

bool Sushi::read_config(const char *fname, bool ok_if_missing)
{
  // Try to open a config file
  std::ifstream config_file(fname);
  if (!config_file)
  {
    if (!ok_if_missing)
    {
      std::perror(fname);
      return false;
    }
    return true;
  }

  // Read the config file
  std::string line;
  while (!config_file.eof())
  {
    line = read_line(config_file);
    if (!parse_command(line))
    {
      store_to_history(line);
    }
  }

  return true;
}

void Sushi::store_to_history(std::string line)
{
  // Do not insert empty lines
  if (line.empty())
  {
    return;
  }

  // Is the history buffer full?
  while (history.size() >= HISTORY_LENGTH)
  {
    history.pop_front();
  }

  history.push_back(line);
}

void Sushi::show_history()
{
  int index = 1;

  // `history` itself will be inserted
  if (history.size() == HISTORY_LENGTH)
  {
    history.pop_front();
  }

  for (const auto &cmd : history)
  {
    std::cout << std::setw(5) << index++ << "  " << cmd << std::endl;
  }

  // `history` itself will be inserted
  std::cout << std::setw(5) << index++ << "  " << "history" << std::endl;
}

void Sushi::set_exit_flag()
{
  exit_flag = true;
}

bool Sushi::get_exit_flag() const
{
  return exit_flag;
}

int Sushi::spawn(Program *exe, bool bg)
{
  std::vector<pid_t> child_pids;
  std::vector<Program *> pipeline;

  Program *current = exe;
  while (current != nullptr)
  {
    pipeline.insert(pipeline.begin(), current);
    current = current->get_pipe();
  }
  int prev[2] = {-1, -1};
  for (size_t i = 0; i < pipeline.size(); ++i)
  {
    Program *current = pipeline[i];

    int pipe_fd[2] = {-1, -1};
    if (i < pipeline.size() - 1)
    {
      // Create pipe for current -> next
      if (pipe(pipe_fd) == -1)
      {
        std::perror("pipe");
        return -1;
      }
    }

    pid_t pid = fork();
    if (pid == -1)
    {
      std::perror("fork");
      return -1;
    }

    if (pid == 0)
    {
      // prev
      Redirection redir = pipeline[i]->get_redir();
      if (redir.get_in() != nullptr)
      {
        int fd = open(redir.get_in()->c_str(), O_RDONLY);
        dup2(fd, STDIN_FILENO);
        close(fd);
      }
        if(redir.get_out1() != nullptr)
      {
        int fd = open(redir.get_out1()->c_str(), O_WRONLY | O_CREAT | O_TRUNC);
        dup2(fd, STDOUT_FILENO);
        close(fd);
      }
      else if(redir.get_out2() != nullptr)
      {
        int fd = open(redir.get_out2()->c_str(), O_WRONLY | O_CREAT | O_APPEND);
        dup2(fd, STDOUT_FILENO);
        close(fd);
      }
      if (prev[0] != -1)
      {
        
        dup2(prev[0], STDIN_FILENO);
        close(prev[0]);
      }
      if (prev[1] != -1)
      {
        close(prev[1]);
      }
      // pipe_fd
      if (pipe_fd[1] != -1)
      {
       
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
      }
      if (pipe_fd[0] != -1)
      {
        close(pipe_fd[0]);
      }
      char *const *argv = current->vector2array();
      execvp(current->progname().c_str(), argv);
      std::perror("execvp");
      exit(EXIT_FAILURE);
    }

    child_pids.push_back(pid);

    if (prev[0] != -1)
    {
      close(prev[0]);
    }
    if (prev[1] != -1)
    {
      close(prev[1]);
    }

    prev[0] = pipe_fd[0];
    prev[1] = pipe_fd[1];
  }

  if (prev[0] != -1)
    close(prev[0]);
  if (prev[1] != -1)
    close(prev[1]);

  int status = 0;
  if (!bg)
  {
    for (pid_t pid : child_pids)
    {
      if (waitpid(pid, &status, 0) == -1)
      {
        perror("waitpid");
      }
    }
    setenv("?", std::to_string(WEXITSTATUS(status)).c_str(), true);
  }
  return 0;
}

void Sushi::prevent_interruption()
{
  struct sigaction sa;
  sa.sa_handler = refuse_to_die;
  // Restart the read() system call
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGINT, &sa, nullptr) != 0)
  {
    std::perror("sigaction");
    std::exit(EXIT_FAILURE);
  }
}

void Sushi::refuse_to_die(int signo)
{
  UNUSED(signo);
  std::cerr << "Type exit to exit the shell" << '\n';
}

void Sushi::mainloop()
{
  while (!get_exit_flag())
  {
    const char *prompt = std::getenv("PS1");
    std::cout << (prompt ? prompt : DEFAULT_PROMPT);
    std::cout.flush(); // Ensure prompt is displayed immediately

    const std::string command = read_line(std::cin);

    if (!parse_command(command) && !re_execute())
    {
      store_to_history(command); // Do not insert the bangs (!)
    }
  }
}

// Two new methods to implement
void Sushi::pwd()
{
  std::cout << getcwd(NULL, 0) << std::endl;
}

void Sushi::cd(std::string *s)
{
  if (chdir(s->c_str()) != 0)
  {
    std::perror("error switching directory");
    return;
  }
  delete s;
}

char *const *Program::vector2array()
{
  // std::vector<std::string*> *args -> char *const argv[]
  assert(args);

  size_t size = args->size();
  char **array = new char *[size + 1]; // Allocate an array of char*

  for (size_t i = 0; i < size; ++i)
  {
    assert((*args)[i]);
    array[i] = const_cast<char *>((*args)[i]->c_str()); // Copy string content
  }

  array[size] = nullptr; // Null-terminate the array
  return array;
}

Program::~Program()
{
  // Do not implement now
}
