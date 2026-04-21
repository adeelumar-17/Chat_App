# Chat App (Qt/C++)

A desktop chat application built with Qt Widgets and C++. The app includes user authentication (login/sign-up with SQLite), real-time messaging over TCP, and basic file attachment transfer.

## Features

- User sign-up and login flow
- SQLite-backed user storage
- Local TCP server/client chat communication
- Public and private message sending
- File attachment send/receive support
- Basic localization setup (`CN_Proj_en_US.ts`)

## Project Structure

- `main.cpp`: Application entry point and translator loading
- `login.*`: Login window and credential validation
- `signup.*`: Sign-up window and account creation
- `database.*`: SQLite setup and user CRUD helpers
- `mainwindow.*`: Chat UI, socket handling, file transfer logic
- `Resources/`: Qt resource files
- `CN_Proj.pro`: qmake project file

## Prerequisites

- Qt 6 (Widgets, Network, SQL modules)
- C++ compiler supported by your Qt kit
- Qt Creator (recommended)

## Build and Run

### Option 1: Qt Creator (recommended)

1. Open `CN_Proj.pro` in Qt Creator.
2. Select a Qt 6 desktop kit.
3. Build the project.
4. Run the app.

### Option 2: Terminal with qmake

```bash
qmake CN_Proj.pro
make
./CN_Proj
```

Note: On some systems, you may need to use `qmake6` instead of `qmake`.

## Notes

- The app stores credentials in a SQLite database file under your app data location (`QStandardPaths::AppDataLocation`).
- Default server/client communication currently uses TCP port `8080`.

## Git Hygiene

This repository includes a `.gitignore` configured to exclude:

- Qt build folders and generated files
- IDE user files
- Local binaries and logs
- Runtime SQLite database files

This keeps pushes lightweight and avoids uploading machine-specific build artifacts.
