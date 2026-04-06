#include <arpa/inet.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <poll.h>
#include <stdlib.h>
#include <sys/poll.h>

int main() {

  int server_fd, client_fd;
  char buffer[1024] = {0};

  struct sockaddr_in addr;
  addr.sin_port = htons(8080);
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd < 0) {
    perror("Socket failed");
    exit(1);
  }

  if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("Bind failed");
    exit(1);
  }

  if (listen(server_fd, 10) < 0) {
    perror("Listen Failed");
    exit(1);
  }
  printf("Server listining on port 8080... \n");

  struct pollfd fds[10];
  for (int i = 0; i < 10; i++) {
    fds[i].fd = -1;
  }
  fds[0].fd = server_fd;
  fds[0].events = POLLIN;

  while (1) {
    if (poll(fds, 10, -1) < 0) {
      perror("Poll");
      exit(1);
    }
    for (int i = 0; i < 10; i++) {
      if (fds[i].fd == -1)
        continue;
      if (fds[i].revents & POLLIN) {
        if (fds[i].fd == server_fd) {
          client_fd = accept(server_fd, NULL, NULL);
          for (int j = 0; j < 10; j++) {
            if (fds[j].fd == -1) {
              fds[j].fd = client_fd;
              fds[j].events = POLLIN;
              break;
            }
          }
        } else {
          memset(buffer, 0, sizeof(buffer));
          int bytes = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
          if (bytes <= 0) {
            close(fds[i].fd);
            fds[i].fd = -1;
          } else {
            printf("Request : \n%s\n", buffer);

            char *status;
            char *content_type;
            char method[50];
            char path[150];
            size_t content_length;
            FILE *fp;

            content_type = "application/octet-stream";
            status = "200 OK";

            sscanf(buffer, "%s %s", method, path);

            if (strcmp(path, "/") == 0) {
              strcpy(path, "/index.html");
            } else if (!strstr(path, ".")) {
              char temp[150];
              snprintf(temp, sizeof(temp), "%s.html", path);
              strcpy(path, temp);
            }

            fp = fopen(path + 1, "rb");

            if (fp == NULL) {
              status = "404 Not Found";
              content_type = "text/html";
              fp = fopen("notFound.html", "rb");
              if (fp == NULL) {
                perror("notFound.html missing");
                exit(1);
              }
              fseek(fp, 0, SEEK_END);
              content_length = ftell(fp);
              fseek(fp, 0, SEEK_SET);

            } else {
              fseek(fp, 0, SEEK_END);
              content_length = ftell(fp);
              fseek(fp, 0, SEEK_SET);

              if (strstr(path, ".html")) {
                content_type = "text/html";
                status = "200 OK";
              } else if (strstr(path, ".css")) {
                content_type = "text/css";
                status = "200 OK";
              }
            }
            char header[250];

            snprintf(header, sizeof(header),
                     "HTTP/1.1 %s\r\n"
                     "Content-Type: %s\r\n"
                     "Content-Length: %zu\r\n"
                     "Connection: close\r\n"
                     "\r\n",
                     status, content_type, content_length);

            send(fds[i].fd, header, strlen(header), 0);

            char file_buffer[4096];
            size_t n;
            while ((n = fread(file_buffer, 1, sizeof(file_buffer), fp)) > 0) {
              size_t total = 0;
              while (total < n) {
                size_t sent =
                    send(fds[i].fd, file_buffer + total, n - total, 0);
                if (sent <= 0) {
                  break;
                }
                total += sent;
              }
            }
            fclose(fp);
            close(fds[i].fd);
            fds[i].fd = -1;
          }
        }
      }
    }
  }
  return 0;
}
