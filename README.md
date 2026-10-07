# Social_Network_Friend_Management_System
# Social Network Management System

A **Social Network Management System** developed in **C language** using **Data Structures and File Handling**.

This project is designed to demonstrate how a real-world social networking application can be implemented using basic concepts of C programming and Data Structures. It provides features such as user registration, login, profile management, friend requests, friend lists, posts, likes, comments, notifications, and permanent data storage.

## 🚀 Features

### 👤 User Management

* Create a new account
* User login and logout
* View own profile
* Edit profile
* Search users
* Display all registered users

The user management system allows users to create accounts and securely access their profiles using their User ID and password. Users can also search for other users and manage their profile information.

### 🤝 Friend Management

* Send friend requests
* Accept or reject friend requests
* View friend list
* Remove friends
* Find mutual friends
* Get friend suggestions

The friend management system allows users to connect with other users. Friend requests can be accepted or rejected, and users can also view their existing friends, mutual friends, and suggested friends.

### 📝 Post Management

* Create posts
* View post feed
* Like posts
* Comment on posts
* Delete your own posts

The post management system allows users to create and view posts in a feed. Users can interact with posts by liking and commenting on them. A user can also delete their own posts.

### 🔔 Notifications

* Friend request notifications
* Friend request acceptance notifications
* Post like notifications
* Post comment notifications
* View notifications

The notification system keeps users informed about important activities related to their account, friends, and posts.

### 💾 Data Storage

The project uses **text files** to store application data. This allows information to remain available even after the program is closed and started again.

Files used include:

* `users.txt`
* `friends.txt`
* `requests.txt`
* `posts.txt`
* `likes.txt`
* `comments.txt`
* `notifications.txt`

## 🧠 Data Structures Used

The project demonstrates several important **Data Structures and C programming concepts**:

* **Singly Linked List**
* **Structures**
* **Pointers**
* **Dynamic Memory Allocation**
* **Searching**
* **Linked List Traversal**
* **File Handling**

Different linked lists are used to manage users, friends, friend requests, posts, likes, comments, and notifications. Structures are used to represent different types of data, while pointers and dynamic memory allocation help manage the linked-list nodes during program execution.

## 🛠️ Technologies Used

* **Programming Language:** C
* **Compiler:** GCC / MinGW
* **Data Structures:** Singly Linked Lists
* **Concepts:** Structures, Pointers, Dynamic Memory Allocation, Searching, Traversal
* **Storage:** Text File Handling
* **Platform:** Windows / Console Application

## 📂 Project Structure

```text
Social-Network-Management-System/
│
├── social.c
├── users.txt
├── friends.txt
├── requests.txt
├── posts.txt
├── likes.txt
├── comments.txt
├── notifications.txt
└── README.md
```

The main source code is contained in `social.c`. The text files are used by the program to save and load different types of user and social-network data.

## 📋 Main Menu

```text
==================================================
       SOCIAL NETWORK MANAGEMENT SYSTEM
==================================================

1. User / Profile
2. Friends
3. Posts
4. Notifications
5. Exit
```

The main menu provides access to all major sections of the application. Users can manage their profiles, friends, posts, and notifications from the respective menus.

## 👤 User / Profile

The User/Profile section provides options for account and profile management:

```text
1. Create Account
2. Login
3. Logout
4. View My Profile
5. Edit My Profile
6. Search User
7. Display All Users
8. Back
```

Users can create their account, login, view or edit their profile, search for other users, and display all registered users.

## 🤝 Friend System

The friend system provides the following operations:

```text
1. Send Friend Request
2. Accept / Reject Request
3. Remove Friend
4. View Friend List
5. View Mutual Friends
6. Friend Suggestions
7. Back
```

This section demonstrates how linked lists can be used to maintain relationships between users and perform operations such as adding, removing, and searching connections.

## 📝 Post Feed

The post section allows users to interact with posts:

```text
1. Create Post
2. Post Feed
3. Like Post
4. Comment on Post
5. Delete Own Post
6. Back
```

The **Post Feed** displays available posts and allows users to interact with them using likes and comments. Users can also delete posts created by themselves.

## 🔄 How the Project Works

When the application starts, previously stored information is loaded from the text files into memory.

Users can then create accounts or login using their existing credentials. After login, they can manage their profile, send friend requests, manage friendships, create posts, like and comment on posts, and view notifications.

The application uses **linked lists** to manage data dynamically during program execution. Each major component, such as users, friends, posts, comments, likes, and notifications, is maintained using appropriate structures and linked-list nodes.

Whenever the application data is updated, the information can be stored in the corresponding text files. This provides basic data persistence, allowing the information to be available again when the program is restarted.

## 🎯 Project Objectives

The main objectives of this project are:

* To understand the practical implementation of **Data Structures in C**.
* To learn how **Singly Linked Lists** can manage dynamic data.
* To understand the use of **Structures and Pointers**.
* To implement **Dynamic Memory Allocation**.
* To use **File Handling** for permanent data storage.
* To develop a menu-driven application.
* To implement basic features of a social networking system.
* To improve programming and problem-solving skills.

## 📚 Concepts Demonstrated

This project combines multiple C programming and DSA concepts into one practical application. It provides experience with linked-list creation and traversal, dynamic memory management, structures, pointers, searching, file operations, and menu-driven programming.

The project shows how these concepts can work together to create a functional application instead of using them only in individual programs.

This project is created for **educational and academic purpose
