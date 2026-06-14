# LazyRESTer

> A terminal-first REST client inspired by LazyGit and LazyVim.

LazyRESTer is a keyboard-driven TUI (Terminal User Interface) for testing and exploring HTTP APIs. It combines the speed of terminal workflows with the convenience of modern API clients, allowing developers to create, organize, execute, and inspect HTTP requests without leaving their terminal.

## Features

### 🚀 Fast Keyboard-Driven Workflow

* Vim-inspired navigation
* Fully keyboard accessible
* Command palette for quick actions
* Customizable keybindings

### 🌐 HTTP Request Management

* Support for GET, POST, PUT, PATCH, DELETE, HEAD, and OPTIONS
* Custom headers and query parameters
* Request body editor with syntax highlighting
* JSON, XML, Form Data, and Raw body support

### 📁 Collections & Workspaces

* Organize requests into collections
* Folder hierarchy support
* Multiple workspaces
* Import and export collections

### 🔐 Environment Variables

* Environment-specific configurations
* Variable interpolation
* Secret management
* Development, staging, and production profiles

Example:

```http
GET {{base_url}}/users/{{user_id}}
Authorization: Bearer {{token}}
```

### 📊 Response Inspection

* Syntax-highlighted response viewer
* Formatted JSON output
* Response headers explorer
* Status code indicators
* Response time and size metrics

### 📝 Request History

* Recent requests
* Request replay
* Searchable history
* Response snapshots

### 🔄 API Development Workflow

* Duplicate and modify requests
* Save responses
* Generate cURL commands
* Import cURL commands
* Export requests as reusable files

## Philosophy

LazyRESTer follows a few simple principles:

* Keyboard first
* Fast startup time
* Local-first configuration
* Git-friendly request storage
* No unnecessary complexity
* Works great over SSH and remote terminals

## Configuration

Requests and collections are stored as plain text files, making them easy to version control and review.


## Roadmap

### v0.1

* Basic HTTP client
* Request collections
* Environment variables
* JSON response viewer

### v0.2

* Request history
* cURL import/export
* Search and filtering
* Custom themes

### v0.3

* GraphQL support
* Authentication helpers
* Response comparison
* Plugin system

### Future

* OpenAPI import
* WebSocket support
* AI-assisted request generation
* Team collaboration features

## Why LazyRESTer?

Modern API tools are powerful but often require a graphical interface. LazyRESTer brings the same productivity to the terminal, allowing developers to stay focused in their existing workflow.

Whether you're debugging a microservice, testing an API, or exploring a new backend, LazyRESTer provides a fast and efficient terminal-native experience.

---

Inspired by LazyGit, LazyDocker, and LazyVim.

