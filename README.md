# 🚀 C HTTP Server (Poll-Based)

A lightweight HTTP server written in pure C using low-level Linux socket APIs and `poll()` for handling multiple clients. This project demonstrates how web servers work under the hood — from accepting connections to parsing HTTP requests and serving static files.

---

## 📌 Features

- 🧠 Built from scratch using C (no frameworks)
- 🔌 Uses `socket`, `bind`, `listen`, and `accept`
- ⚡ Handles multiple clients using `poll()`
- 📄 Serves static files (`.html`, `.css`)
- 🔍 Basic HTTP request parsing (method + path)
- ❌ Custom 404 page support
- 📦 Sends proper HTTP headers (`Content-Type`, `Content-Length`)

---

## 🛠️ How It Works

1. Creates a TCP socket
2. Binds to port `8080`
3. Listens for incoming connections
4. Uses `poll()` to monitor multiple file descriptors
5. Accepts new clients dynamically
6. Reads HTTP requests
7. Parses the request line (`GET /path`)
8. Maps paths to files:
   - `/` → `index.html`
   - `/about` → `about.html`

9. Opens and reads requested files
10. Sends HTTP response headers + file content
11. Closes connection

---

## 📂 Project Structure

```
.
├── server.c
├── index.html
├── about.html
├── styles.css
├── notFound.html
└── README.md
```

---

## ⚙️ Compilation & Run

### 🔧 Compile

```bash
gcc server.c -o server
```

### ▶️ Run

```bash
./server
```

Server will start on:

```
http://localhost:8080
```

---

## 🌐 Example Requests

| URL           | Result                        |
| ------------- | ----------------------------- |
| `/`           | Loads `index.html`            |
| `/about`      | Loads `about.html`            |
| `/styles.css` | Loads CSS file                |
| `/random`     | Loads `random.html` if exists |
| `/notexist`   | Loads `notFound.html`         |

---

## 📄 Supported Content Types

| Extension | Content-Type               |
| --------- | -------------------------- |
| `.html`   | `text/html`                |
| `.css`    | `text/css`                 |
| others    | `application/octet-stream` |

---

## 🧠 What You Learn From This

- How HTTP works internally
- Low-level networking in C
- Handling multiple clients with `poll()`
- File I/O (`fopen`, `fread`, `fseek`, `ftell`)
- Building a minimal web server from scratch

---

## 📜 License

This project is open-source and free to use for learning purposes.

---

## 🙌 Acknowledgments

Inspired by the desire to understand how web servers actually work behind frameworks like Node.js, Express, and Nginx.

---
