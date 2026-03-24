## OctoLink
**secure messenger for friends and family**

## About

OctoLink is an end-to-end encrypted messenger designed for private communication.  
Built with modern C++ and Boost.Asio, it combines security, performance, and cross-platform support.

---

## Features
- End-to-end encryption (E2EE) — server never sees your keys
- Private & group chats
- File sharing
- Voice & video calls (coming soon)
- Cross-platform: Desktop (Qt), Web (React), Mobile (Kotlin/Swift)

## Current Stack 
### Backend
- **C++20** — modern C++ (RAII, move semantics, concepts)
- **Clang / GCC**
- **Boost.Asio** — async networking

### Frontend
- *In development*

### DevOps
- *In development*

---

## Future Stack

### Backend
- **PostgreSQL** — persistent storage
- **Redis** — caching, sessions
- **gRPC** — microservices

### Frontend
- **Qt** — desktop GUI
- **React** — web client
- **Kotlin / Swift** — mobile apps

### DevOps
- **Docker** — containerization
- **Kubernetes** — orchestration
- **NGINX** — reverse proxy
- **Kafka** — message queue
- **CI/CD** — GitHub Actions

### Tools
- **CMake** — build system
- **gTest** — unit tests

---

## Build & Run

```bash
git clone git@github.com:TSX97/OctoLink.git
cd OctoLink
mkdir build && cd build
cmake ..
make
./octolink_server
```

