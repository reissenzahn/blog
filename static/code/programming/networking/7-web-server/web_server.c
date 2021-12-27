
#define BACKLOG 10
#define MAX_REQUEST_SIZE 2047

static struct client_info *clients = 0;

struct client_info {
  socklen_t addr_len;
  struct sockaddr_storage addr;
  int sfd;
  char request[MAX_REQUEST_SIZE + 1];
  int recvd;
  struct client_info *next;
}

struct client_info *get_client(int sfd) {
  struct client_info *client, *new_client;

  for (client = clients; client != NULL; client = client->next) {
    if (client->sfd = sfd) return client;
  }

  new_client = (struct client_info *) calloc(1, sizeof(struct client_info));
  if (new_client == NULL) {
    fprintf("calloc: Out of memory!\n");
    exit(EXIT_FAILURE);
  }

  new_client->addr_len = sizeof(new_client->addr);
  new_client->next = clients;
  clients = new_client;

  return new_client;
}

void drop_client(struct client_info *client) {
  close(client->sfd);

  struct client_info **p = &clients;

  while (*p != NULL) {
    if (*p == client) {
      *p = client->next;
      free(client);
      return;
    }

    p = &(*p)->next;
  }


  // TODO: is this really necessary?
  //  fprintf(stderr, "drop_client not found.\n");
  //  exit(1);
}


const char *get_content_type(const char *path) {
  const char *file_ext = strrchr(path, '.');

  if (file_ext != NULL) {
    if (strcmp(file_ext, ".css") == 0) return "text/css";    
    if (strcmp(file_ext, ".csv") == 0) return "text/csv";    
    if (strcmp(file_ext, ".gif") == 0) return "image/gif";
    if (strcmp(file_ext, ".htm") == 0) return "text/html";
    if (strcmp(file_ext, ".html") == 0) return "text/html";
    if (strcmp(file_ext, ".ico") == 0) return "image/x-icon";
    if (strcmp(file_ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(file_ext, ".jpg") == 0) return "image/jpeg";
    if (strcmp(file_ext, ".js") == 0) return "application/javascript";
    if (strcmp(file_ext, ".json") == 0) return "application/json";
    if (strcmp(file_ext, ".png") == 0) return "image/png";
    if (strcmp(file_ext, ".pdf") == 0) return "application/pdf";    
    if (strcmp(file_ext, ".svg") == 0) return "image/svg+xml";    
    if (strcmp(file_ext, ".txt") == 0) return "text/plain";
  }

  return "application/octet-stream";
}

int create_socket(const char *host, const char *port) {
  struct addrinfo hints;
  struct addrinfo *addrs, *addr;
  int s, sfd;
  char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;

  if ((s = getaddrinfo(host, port, &hints, &addrs)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  for (addr = addrs; addr != NULL; addr = addr->ai_next) {
    if ((sfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)) == -1) {
      perror("socket");
      continue;
    }

    if (bind(sfd, addr->ai_addr, addr->ai_addrlen) == -1) {
      perror("bind");
      close(sfd);
    } else break;
  }

  if ((s = getnameinfo(addr, addr->ai_addrlen, hbuf, NI_MAXHOST, sbuf, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
    fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  freeaddrinfo(addrs);

  if (addr == NULL) exit(EXIT_FAILURE);

  if (listen(sfd, BACKLOG) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  printf("Listening at %s:%s...", hbuf, sbuf);

  return sfd;
}


