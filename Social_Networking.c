#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT 300

/* =========================================================
   STRUCTURES
   ========================================================= */

typedef struct Friend
{
    int userID;
    struct Friend *next;
} Friend;

typedef struct User
{
    int id;
    char name[50];
    int age;
    char city[50];
    char password[50];

    int loggedIn;

    Friend *friends;
    struct User *next;
} User;

typedef struct Request
{
    int senderID;
    int receiverID;
    struct Request *next;
} Request;

typedef struct LikedUser
{
    int userID;
    struct LikedUser *next;
} LikedUser;

typedef struct Comment
{
    int userID;
    char text[MAX_TEXT];
    struct Comment *next;
} Comment;

typedef struct Post
{
    int postID;
    int userID;
    char text[MAX_TEXT];

    int likes;

    LikedUser *likedUsers;
    Comment *comments;

    struct Post *next;
} Post;

typedef struct Notification
{
    int receiverID;
    char message[300];

    struct Notification *next;
} Notification;


/* =========================================================
   GLOBAL VARIABLES
   ========================================================= */

User *userHead = NULL;
Request *requestHead = NULL;
Post *postHead = NULL;
Notification *notificationHead = NULL;

int nextUserID = 1;
int nextPostID = 1;


/* =========================================================
   USER FIND
   ========================================================= */

User *findUser(int id)
{
    User *temp = userHead;

    while (temp != NULL)
    {
        if (temp->id == id)
            return temp;

        temp = temp->next;
    }

    return NULL;
}


/* =========================================================
   POST FIND
   ========================================================= */

Post *findPost(int id)
{
    Post *temp = postHead;

    while (temp != NULL)
    {
        if (temp->postID == id)
            return temp;

        temp = temp->next;
    }

    return NULL;
}


/* =========================================================
   FRIEND FUNCTIONS
   ========================================================= */

int areFriends(User *user, int friendID)
{
    Friend *temp = user->friends;

    while (temp != NULL)
    {
        if (temp->userID == friendID)
            return 1;

        temp = temp->next;
    }

    return 0;
}


void addFriend(User *u1, User *u2)
{
    Friend *newFriend;

    if (!areFriends(u1, u2->id))
    {
        newFriend = (Friend *)malloc(sizeof(Friend));

        newFriend->userID = u2->id;
        newFriend->next = u1->friends;

        u1->friends = newFriend;
    }

    if (!areFriends(u2, u1->id))
    {
        newFriend = (Friend *)malloc(sizeof(Friend));

        newFriend->userID = u1->id;
        newFriend->next = u2->friends;

        u2->friends = newFriend;
    }
}


void removeFriendFromList(User *user, int friendID)
{
    Friend *temp = user->friends;
    Friend *prev = NULL;

    while (temp != NULL)
    {
        if (temp->userID == friendID)
        {
            if (prev == NULL)
                user->friends = temp->next;
            else
                prev->next = temp->next;

            free(temp);
            return;
        }

        prev = temp;
        temp = temp->next;
    }
}


int countMutualFriends(User *u1, User *u2)
{
    Friend *temp = u1->friends;
    int count = 0;

    while (temp != NULL)
    {
        if (areFriends(u2, temp->userID))
            count++;

        temp = temp->next;
    }

    return count;
}


/* =========================================================
   SAVE FRIENDS
   ========================================================= */

void saveFriends()
{
    FILE *fp;
    User *user;
    Friend *f;

    fp = fopen("friends.txt", "w");

    if (fp == NULL)
    {
        printf("Error opening friends.txt\n");
        return;
    }

    user = userHead;

    while (user != NULL)
    {
        f = user->friends;

        while (f != NULL)
        {
            /*
               Save only one side to avoid duplicate
               friendship records.
            */
            if (user->id < f->userID)
            {
                fprintf(fp, "%d %d\n", user->id, f->userID);
            }

            f = f->next;
        }

        user = user->next;
    }

    fclose(fp);
}


/* =========================================================
   LOAD FRIENDS
   ========================================================= */

void loadFriends()
{
    FILE *fp;
    int id1, id2;

    User *u1;
    User *u2;

    fp = fopen("friends.txt", "r");

    if (fp == NULL)
        return;

    while (fscanf(fp, "%d %d", &id1, &id2) == 2)
    {
        u1 = findUser(id1);
        u2 = findUser(id2);

        if (u1 != NULL && u2 != NULL)
        {
            addFriend(u1, u2);
        }
    }

    fclose(fp);
}


/* =========================================================
   USER DATA SAVE
   ========================================================= */

void saveUsers()
{
    FILE *fp;
    User *temp;

    fp = fopen("users.txt", "w");

    if (fp == NULL)
    {
        printf("Error opening users.txt\n");
        return;
    }

    temp = userHead;

    while (temp != NULL)
    {
        fprintf(fp, "%d|%s|%d|%s|%s\n",
                temp->id,
                temp->name,
                temp->age,
                temp->city,
                temp->password);

        temp = temp->next;
    }

    fclose(fp);
}


/* =========================================================
   USER DATA LOAD
   ========================================================= */

void loadUsers()
{
    FILE *fp;
    User *newUser;
    User *temp;

    fp = fopen("users.txt", "r");

    if (fp == NULL)
        return;

    while (1)
    {
        newUser = (User *)malloc(sizeof(User));

        if (fscanf(fp,
                   "%d|%49[^|]|%d|%49[^|]|%49[^\n]\n",
                   &newUser->id,
                   newUser->name,
                   &newUser->age,
                   newUser->city,
                   newUser->password) != 5)
        {
            free(newUser);
            break;
        }

        newUser->loggedIn = 0;
        newUser->friends = NULL;
        newUser->next = NULL;

        if (userHead == NULL)
        {
            userHead = newUser;
        }
        else
        {
            temp = userHead;

            while (temp->next != NULL)
                temp = temp->next;

            temp->next = newUser;
        }

        if (newUser->id >= nextUserID)
            nextUserID = newUser->id + 1;
    }

    fclose(fp);
}


/* =========================================================
   REQUEST SAVE
   ========================================================= */

void saveRequests()
{
    FILE *fp;
    Request *temp;

    fp = fopen("requests.txt", "w");

    if (fp == NULL)
        return;

    temp = requestHead;

    while (temp != NULL)
    {
        fprintf(fp, "%d %d\n",
                temp->senderID,
                temp->receiverID);

        temp = temp->next;
    }

    fclose(fp);
}


/* =========================================================
   REQUEST LOAD
   ========================================================= */

void loadRequests()
{
    FILE *fp;
    Request *newRequest;
    Request *temp;

    fp = fopen("requests.txt", "r");

    if (fp == NULL)
        return;

    while (1)
    {
        newRequest =
            (Request *)malloc(sizeof(Request));

        if (fscanf(fp,
                   "%d %d",
                   &newRequest->senderID,
                   &newRequest->receiverID) != 2)
        {
            free(newRequest);
            break;
        }

        newRequest->next = NULL;

        if (requestHead == NULL)
        {
            requestHead = newRequest;
        }
        else
        {
            temp = requestHead;

            while (temp->next != NULL)
                temp = temp->next;

            temp->next = newRequest;
        }
    }

    fclose(fp);
}


/* =========================================================
   REQUEST EXISTS
   ========================================================= */

int requestExists(int senderID, int receiverID)
{
    Request *temp = requestHead;

    while (temp != NULL)
    {
        if (temp->senderID == senderID &&
            temp->receiverID == receiverID)
        {
            return 1;
        }

        temp = temp->next;
    }

    return 0;
}


/* =========================================================
   NOTIFICATION
   ========================================================= */

void addNotification(int receiverID, const char *message)
{
    Notification *newNotification;
    Notification *temp;

    newNotification =
        (Notification *)malloc(sizeof(Notification));

    newNotification->receiverID = receiverID;

    strcpy(newNotification->message, message);

    newNotification->next = NULL;

    if (notificationHead == NULL)
    {
        notificationHead = newNotification;
    }
    else
    {
        temp = notificationHead;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newNotification;
    }
}


void saveNotifications()
{
    FILE *fp;
    Notification *temp;

    fp = fopen("notifications.txt", "w");

    if (fp == NULL)
        return;

    temp = notificationHead;

    while (temp != NULL)
    {
        fprintf(fp,
                "%d|%s\n",
                temp->receiverID,
                temp->message);

        temp = temp->next;
    }

    fclose(fp);
}


void loadNotifications()
{
    FILE *fp;
    Notification *newNotification;
    Notification *temp;

    fp = fopen("notifications.txt", "r");

    if (fp == NULL)
        return;

    while (1)
    {
        newNotification =
            (Notification *)malloc(sizeof(Notification));

        if (fscanf(fp,
                   "%d|%299[^\n]\n",
                   &newNotification->receiverID,
                   newNotification->message) != 2)
        {
            free(newNotification);
            break;
        }

        newNotification->next = NULL;

        if (notificationHead == NULL)
        {
            notificationHead = newNotification;
        }
        else
        {
            temp = notificationHead;

            while (temp->next != NULL)
                temp = temp->next;

            temp->next = newNotification;
        }
    }

    fclose(fp);
}


void viewNotifications(User *currentUser)
{
    Notification *temp;
    int found = 0;

    printf("\n============================================\n");
    printf("              NOTIFICATIONS\n");
    printf("============================================\n");

    temp = notificationHead;

    while (temp != NULL)
    {
        if (temp->receiverID == currentUser->id)
        {
            printf("?? %s\n", temp->message);
            found = 1;
        }

        temp = temp->next;
    }

    if (!found)
        printf("No notifications.\n");
}


/* =========================================================
   CREATE ACCOUNT
   ========================================================= */

void createAccount()
{
    User *newUser;
    User *temp;

    newUser = (User *)malloc(sizeof(User));

    newUser->id = nextUserID++;

    printf("\nEnter Name: ");
    scanf(" %[^\n]", newUser->name);

    printf("Enter Age: ");
    scanf("%d", &newUser->age);

    printf("Enter City: ");
    scanf(" %[^\n]", newUser->city);

    printf("Create Password: ");
    scanf(" %[^\n]", newUser->password);

    newUser->loggedIn = 0;
    newUser->friends = NULL;
    newUser->next = NULL;

    if (userHead == NULL)
    {
        userHead = newUser;
    }
    else
    {
        temp = userHead;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newUser;
    }

    saveUsers();

    printf("\nAccount created successfully!\n");
    printf("Your User ID is: %d\n", newUser->id);
}


/* =========================================================
   LOGIN
   ========================================================= */

User *login()
{
    int id;
    char password[50];

    User *user;

    printf("\nEnter User ID: ");
    scanf("%d", &id);

    user = findUser(id);

    if (user == NULL)
    {
        printf("User not found.\n");
        return NULL;
    }

    printf("Enter Password: ");
    scanf(" %[^\n]", password);

    if (strcmp(user->password, password) == 0)
    {
        user->loggedIn = 1;

        printf("\nLogin successful!\n");
        printf("Welcome %s!\n", user->name);

        return user;
    }

    printf("Wrong password.\n");

    return NULL;
}


/* =========================================================
   LOGOUT
   ========================================================= */

void logout(User *currentUser)
{
    if (currentUser != NULL)
    {
        currentUser->loggedIn = 0;

        printf("Logout successful.\n");
    }
}


/* =========================================================
   PROFILE
   ========================================================= */

void viewMyProfile(User *currentUser)
{
    Friend *temp;
    int count = 0;

    printf("\n============================================\n");
    printf("                MY PROFILE\n");
    printf("============================================\n");

    printf("User ID : %d\n", currentUser->id);
    printf("Name    : %s\n", currentUser->name);
    printf("Age     : %d\n", currentUser->age);
    printf("City    : %s\n", currentUser->city);

    temp = currentUser->friends;

    while (temp != NULL)
    {
        count++;
        temp = temp->next;
    }

    printf("Friends : %d\n", count);
}


void editMyProfile(User *currentUser)
{
    printf("\nEnter New Name: ");
    scanf(" %[^\n]", currentUser->name);

    printf("Enter New Age: ");
    scanf("%d", &currentUser->age);

    printf("Enter New City: ");
    scanf(" %[^\n]", currentUser->city);

    printf("Change Password? (Y/N): ");

    char choice;
    scanf(" %c", &choice);

    if (choice == 'Y' || choice == 'y')
    {
        printf("Enter New Password: ");
        scanf(" %[^\n]", currentUser->password);
    }

    saveUsers();

    printf("Profile updated successfully.\n");
}


/* =========================================================
   SEARCH USER
   ========================================================= */

void searchUser()
{
    char name[50];
    User *temp;
    int found = 0;

    printf("\nEnter name to search: ");
    scanf(" %[^\n]", name);

    temp = userHead;

    while (temp != NULL)
    {
        if (strstr(temp->name, name) != NULL)
        {
            printf("\nID   : %d", temp->id);
            printf("\nName : %s", temp->name);
            printf("\nCity : %s\n", temp->city);

            found = 1;
        }

        temp = temp->next;
    }

    if (!found)
        printf("No user found.\n");
}


/* =========================================================
   DISPLAY USERS
   ========================================================= */

void displayUsers()
{
    User *temp = userHead;

    printf("\n============================================\n");
    printf("                 ALL USERS\n");
    printf("============================================\n");

    if (temp == NULL)
    {
        printf("No users available.\n");
        return;
    }

    while (temp != NULL)
    {
        printf("ID: %d | Name: %s | Age: %d | City: %s\n",
               temp->id,
               temp->name,
               temp->age,
               temp->city);

        temp = temp->next;
    }
}


/* =========================================================
   SEND FRIEND REQUEST
   ========================================================= */

void sendFriendRequest(User *currentUser)
{
    int receiverID;

    User *receiver;

    Request *newRequest;
    Request *temp;

    char message[300];

    printf("\nEnter User ID: ");
    scanf("%d", &receiverID);

    receiver = findUser(receiverID);

    if (receiver == NULL)
    {
        printf("User not found.\n");
        return;
    }

    if (receiverID == currentUser->id)
    {
        printf("You cannot send request to yourself.\n");
        return;
    }

    if (areFriends(currentUser, receiverID))
    {
        printf("Already friends.\n");
        return;
    }

    if (requestExists(currentUser->id, receiverID))
    {
        printf("Request already sent.\n");
        return;
    }

    if (requestExists(receiverID, currentUser->id))
    {
        printf("This user has already sent you a request.\n");
        return;
    }

    newRequest =
        (Request *)malloc(sizeof(Request));

    newRequest->senderID = currentUser->id;
    newRequest->receiverID = receiverID;
    newRequest->next = NULL;

    if (requestHead == NULL)
    {
        requestHead = newRequest;
    }
    else
    {
        temp = requestHead;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newRequest;
    }

    saveRequests();

    sprintf(message,
            "%s sent you a friend request.",
            currentUser->name);

    addNotification(receiverID, message);
    saveNotifications();

    printf("Friend request sent successfully.\n");
}


/* =========================================================
   ACCEPT / REJECT REQUEST
   ========================================================= */

void receivedRequests(User *currentUser)
{
    Request *temp;
    Request *prev;
    Request *deleteNode;

    int found = 0;
    char choice;

    temp = requestHead;
    prev = NULL;

    printf("\n============================================\n");
    printf("            FRIEND REQUESTS\n");
    printf("============================================\n");

    while (temp != NULL)
    {
        if (temp->receiverID == currentUser->id)
        {
            User *sender = findUser(temp->senderID);

            if (sender != NULL)
            {
                found = 1;

                printf("\n%s (ID: %d) sent you a request.\n",
                       sender->name,
                       sender->id);

                printf("Accept? (Y/N): ");
                scanf(" %c", &choice);

                if (choice == 'Y' || choice == 'y')
                {
                    char message[300];

                    addFriend(currentUser, sender);
                    saveFriends();

                    sprintf(message,
                            "%s accepted your friend request.",
                            currentUser->name);

                    addNotification(sender->id, message);
                    saveNotifications();

                    printf("Request accepted.\n");
                }
                else
                {
                    printf("Request rejected.\n");
                }

                deleteNode = temp;

                if (prev == NULL)
                    requestHead = temp->next;
                else
                    prev->next = temp->next;

                temp = temp->next;

                free(deleteNode);

                saveRequests();

                continue;
            }
        }

        prev = temp;
        temp = temp->next;
    }

    if (!found)
        printf("No pending requests.\n");
}


/* =========================================================
   FRIEND LIST
   ========================================================= */

void displayFriendList(User *currentUser)
{
    Friend *temp;
    User *friendUser;

    printf("\n============================================\n");
    printf("                 FRIEND LIST\n");
    printf("============================================\n");

    temp = currentUser->friends;

    if (temp == NULL)
    {
        printf("No friends yet.\n");
        return;
    }

    while (temp != NULL)
    {
        friendUser = findUser(temp->userID);

        if (friendUser != NULL)
        {
            printf("ID: %d | Name: %s | City: %s\n",
                   friendUser->id,
                   friendUser->name,
                   friendUser->city);
        }

        temp = temp->next;
    }
}


/* =========================================================
   REMOVE FRIEND
   ========================================================= */

void removeFriend(User *currentUser)
{
    int friendID;

    User *friendUser;

    printf("\nEnter Friend ID: ");
    scanf("%d", &friendID);

    friendUser = findUser(friendID);

    if (friendUser == NULL)
    {
        printf("User not found.\n");
        return;
    }

    if (!areFriends(currentUser, friendID))
    {
        printf("This user is not your friend.\n");
        return;
    }

    removeFriendFromList(currentUser, friendID);
    removeFriendFromList(friendUser, currentUser->id);

    saveFriends();

    printf("Friend removed successfully.\n");
}


/* =========================================================
   MUTUAL FRIENDS
   ========================================================= */

void viewMutualFriends()
{
    int id1, id2;
    int count = 0;

    User *u1;
    User *u2;

    Friend *temp;

    printf("\nEnter First User ID: ");
    scanf("%d", &id1);

    printf("Enter Second User ID: ");
    scanf("%d", &id2);

    u1 = findUser(id1);
    u2 = findUser(id2);

    if (u1 == NULL || u2 == NULL)
    {
        printf("User not found.\n");
        return;
    }

    printf("\n============================================\n");
    printf("              MUTUAL FRIENDS\n");
    printf("============================================\n");

    temp = u1->friends;

    while (temp != NULL)
    {
        if (areFriends(u2, temp->userID))
        {
            User *mutual = findUser(temp->userID);

            if (mutual != NULL)
            {
                printf("ID: %d | %s\n",
                       mutual->id,
                       mutual->name);

                count++;
            }
        }

        temp = temp->next;
    }

    if (count == 0)
        printf("No mutual friends found.\n");
    else
        printf("\nTotal Mutual Friends: %d\n", count);
}


/* =========================================================
   FRIEND SUGGESTIONS
   ========================================================= */

void friendSuggestions(User *currentUser)
{
    User *temp = userHead;
    int found = 0;

    printf("\n============================================\n");
    printf("             FRIEND SUGGESTIONS\n");
    printf("============================================\n");

    while (temp != NULL)
    {
        if (temp->id != currentUser->id &&
            !areFriends(currentUser, temp->id))
        {
            int mutual =
                countMutualFriends(currentUser, temp);

            if (mutual > 0)
            {
                printf("ID: %d | %s | %d mutual friend(s)\n",
                       temp->id,
                       temp->name,
                       mutual);

                found = 1;
            }
        }

        temp = temp->next;
    }

    if (!found)
        printf("No friend suggestions available.\n");
}


/* =========================================================
   CREATE POST
   ========================================================= */

void createPost(User *currentUser)
{
    Post *newPost;
    Post *temp;

    newPost = (Post *)malloc(sizeof(Post));

    newPost->postID = nextPostID++;
    newPost->userID = currentUser->id;

    printf("\nWrite your post: ");
    scanf(" %[^\n]", newPost->text);

    newPost->likes = 0;
    newPost->likedUsers = NULL;
    newPost->comments = NULL;
    newPost->next = NULL;

    if (postHead == NULL)
    {
        postHead = newPost;
    }
    else
    {
        temp = postHead;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newPost;
    }

    printf("Post created successfully.\n");
}


/* =========================================================
   SAVE POSTS
   ========================================================= */

void savePosts()
{
    FILE *fp;
    Post *temp;

    fp = fopen("posts.txt", "w");

    if (fp == NULL)
        return;

    temp = postHead;

    while (temp != NULL)
    {
        fprintf(fp,
                "%d|%d|%s\n",
                temp->postID,
                temp->userID,
                temp->text);

        temp = temp->next;
    }

    fclose(fp);
}


/* =========================================================
   LOAD POSTS
   ========================================================= */

void loadPosts()
{
    FILE *fp;
    Post *newPost;
    Post *temp;

    fp = fopen("posts.txt", "r");

    if (fp == NULL)
        return;

    while (1)
    {
        newPost = (Post *)malloc(sizeof(Post));

        if (fscanf(fp,
                   "%d|%d|%299[^\n]\n",
                   &newPost->postID,
                   &newPost->userID,
                   newPost->text) != 3)
        {
            free(newPost);
            break;
        }

        newPost->likes = 0;
        newPost->likedUsers = NULL;
        newPost->comments = NULL;
        newPost->next = NULL;

        if (postHead == NULL)
        {
            postHead = newPost;
        }
        else
        {
            temp = postHead;

            while (temp->next != NULL)
                temp = temp->next;

            temp->next = newPost;
        }

        if (newPost->postID >= nextPostID)
            nextPostID = newPost->postID + 1;
    }

    fclose(fp);
}


/* =========================================================
   LIKE FUNCTIONS
   ========================================================= */

int alreadyLiked(Post *post, int userID)
{
    LikedUser *temp = post->likedUsers;

    while (temp != NULL)
    {
        if (temp->userID == userID)
            return 1;

        temp = temp->next;
    }

    return 0;
}


void saveLikes()
{
    FILE *fp;
    Post *post;
    LikedUser *like;

    fp = fopen("likes.txt", "w");

    if (fp == NULL)
        return;

    post = postHead;

    while (post != NULL)
    {
        like = post->likedUsers;

        while (like != NULL)
        {
            fprintf(fp,
                    "%d %d\n",
                    post->postID,
                    like->userID);

            like = like->next;
        }

        post = post->next;
    }

    fclose(fp);
}


void loadLikes()
{
    FILE *fp;
    int postID;
    int userID;

    Post *post;
    LikedUser *newLike;

    fp = fopen("likes.txt", "r");

    if (fp == NULL)
        return;

    while (fscanf(fp, "%d %d",
                  &postID,
                  &userID) == 2)
    {
        post = findPost(postID);

        if (post != NULL &&
            !alreadyLiked(post, userID))
        {
            newLike =
                (LikedUser *)malloc(sizeof(LikedUser));

            newLike->userID = userID;
            newLike->next = post->likedUsers;

            post->likedUsers = newLike;

            post->likes++;
        }
    }

    fclose(fp);
}


void likePost(User *currentUser, Post *post)
{
    LikedUser *newLike;

    User *owner;

    char message[300];

    if (alreadyLiked(post, currentUser->id))
    {
        printf("You already liked this post.\n");
        return;
    }

    newLike =
        (LikedUser *)malloc(sizeof(LikedUser));

    newLike->userID = currentUser->id;
    newLike->next = post->likedUsers;

    post->likedUsers = newLike;

    post->likes++;

    saveLikes();

    owner = findUser(post->userID);

    if (owner != NULL &&
        owner->id != currentUser->id)
    {
        sprintf(message,
                "%s liked your post.",
                currentUser->name);

        addNotification(owner->id, message);
        saveNotifications();
    }

    printf("?? Post liked successfully!\n");
    printf("?? Likes: %d\n", post->likes);
}


/* =========================================================
   COMMENTS
   ========================================================= */

void saveComments()
{
    FILE *fp;
    Post *post;
    Comment *comment;

    fp = fopen("comments.txt", "w");

    if (fp == NULL)
        return;

    post = postHead;

    while (post != NULL)
    {
        comment = post->comments;

        while (comment != NULL)
        {
            fprintf(fp,
                    "%d|%d|%s\n",
                    post->postID,
                    comment->userID,
                    comment->text);

            comment = comment->next;
        }

        post = post->next;
    }

    fclose(fp);
}


void loadComments()
{
    FILE *fp;

    int postID;
    int userID;

    char text[MAX_TEXT];

    Post *post;
    Comment *newComment;
    Comment *temp;

    fp = fopen("comments.txt", "r");

    if (fp == NULL)
        return;

    while (fscanf(fp,
                  "%d|%d|%299[^\n]\n",
                  &postID,
                  &userID,
                  text) == 3)
    {
        post = findPost(postID);

        if (post != NULL)
        {
            newComment =
                (Comment *)malloc(sizeof(Comment));

            newComment->userID = userID;

            strcpy(newComment->text, text);

            newComment->next = NULL;

            if (post->comments == NULL)
            {
                post->comments = newComment;
            }
            else
            {
                temp = post->comments;

                while (temp->next != NULL)
                    temp = temp->next;

                temp->next = newComment;
            }
        }
    }

    fclose(fp);
}


void commentOnPost(User *currentUser, Post *post)
{
    Comment *newComment;
    Comment *temp;

    User *owner;

    char message[300];

    newComment =
        (Comment *)malloc(sizeof(Comment));

    newComment->userID = currentUser->id;

    printf("Enter your comment: ");
    scanf(" %[^\n]", newComment->text);

    newComment->next = NULL;

    if (post->comments == NULL)
    {
        post->comments = newComment;
    }
    else
    {
        temp = post->comments;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newComment;
    }

    saveComments();

    owner = findUser(post->userID);

    if (owner != NULL &&
        owner->id != currentUser->id)
    {
        sprintf(message,
                "%s commented on your post.",
                currentUser->name);

        addNotification(owner->id, message);
        saveNotifications();
    }

    printf("?? Comment added successfully!\n");
}


void displayComments(Post *post)
{
    Comment *temp;
    User *user;

    temp = post->comments;

    if (temp == NULL)
    {
        printf("No comments yet.\n");
        return;
    }

    printf("\nComments:\n");

    while (temp != NULL)
    {
        user = findUser(temp->userID);

        if (user != NULL)
        {
            printf("  %s: %s\n",
                   user->name,
                   temp->text);
        }

        temp = temp->next;
    }
}


/* =========================================================
   FACEBOOK STYLE POST FEED
   ========================================================= */

void postFeed(User *currentUser)
{
    Post *temp;
    User *owner;

    int choice;

    temp = postHead;

    if (temp == NULL)
    {
        printf("\nNo posts available.\n");
        return;
    }

    while (temp != NULL)
    {
        owner = findUser(temp->userID);

        printf("\n\n");
        printf("==================================================\n");

        if (owner != NULL)
            printf("?? %s\n", owner->name);

        printf("--------------------------------------------------\n");

        printf("%s\n\n", temp->text);

        printf("?? %d Likes\n", temp->likes);

        displayComments(temp);

        printf("\n--------------------------------------------------\n");
        printf("1. ?? Like\n");
        printf("2. ?? Comment\n");
        printf("3. Next Post\n");
        printf("4. Back to Post Menu\n");

        printf("\nChoose: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            likePost(currentUser, temp);
            break;

        case 2:
            commentOnPost(currentUser, temp);
            break;

        case 3:
            temp = temp->next;
            break;

        case 4:
            return;

        default:
            printf("Invalid choice.\n");
        }
    }

    printf("\n========== END OF POST FEED ==========\n");
}


/* =========================================================
   LIKE SPECIFIC POST
   ========================================================= */

void likeSpecificPost(User *currentUser)
{
    int postID;

    Post *post;

    printf("\nEnter Post ID: ");
    scanf("%d", &postID);

    post = findPost(postID);

    if (post == NULL)
    {
        printf("Post not found.\n");
        return;
    }

    likePost(currentUser, post);
}


/* =========================================================
   COMMENT SPECIFIC POST
   ========================================================= */

void commentSpecificPost(User *currentUser)
{
    int postID;

    Post *post;

    printf("\nEnter Post ID: ");
    scanf("%d", &postID);

    post = findPost(postID);

    if (post == NULL)
    {
        printf("Post not found.\n");
        return;
    }

    commentOnPost(currentUser, post);
}


/* =========================================================
   DELETE OWN POST
   ========================================================= */

void deleteOwnPost(User *currentUser)
{
    int postID;

    Post *temp;
    Post *prev;

    printf("\nEnter Post ID: ");
    scanf("%d", &postID);

    temp = postHead;
    prev = NULL;

    while (temp != NULL)
    {
        if (temp->postID == postID)
        {
            if (temp->userID != currentUser->id)
            {
                printf("You can delete only your own post.\n");
                return;
            }

            if (prev == NULL)
                postHead = temp->next;
            else
                prev->next = temp->next;

            free(temp);

            savePosts();

            /*
               Likes/comments files are rewritten so that
               deleted post's old data disappears.
            */
            saveLikes();
            saveComments();

            printf("Post deleted successfully.\n");

            return;
        }

        prev = temp;
        temp = temp->next;
    }

    printf("Post not found.\n");
}


/* =========================================================
   USER / PROFILE MENU
   ========================================================= */

void userMenu(User **currentUser)
{
    int choice;

    do
    {
        printf("\n\n");
        printf("============================================\n");
        printf("            ?? USER / PROFILE\n");
        printf("============================================\n");

        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Logout\n");
        printf("4. View My Profile\n");
        printf("5. Edit My Profile\n");
        printf("6. Search User\n");
        printf("7. Display All Users\n");
        printf("8. Back\n");

        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            createAccount();
            break;

        case 2:
            if (*currentUser != NULL &&
                (*currentUser)->loggedIn)
            {
                printf("A user is already logged in.\n");
            }
            else
            {
                *currentUser = login();
            }
            break;

        case 3:
            if (*currentUser != NULL)
            {
                logout(*currentUser);
                *currentUser = NULL;
            }
            else
            {
                printf("No user logged in.\n");
            }
            break;

        case 4:
            if (*currentUser != NULL &&
                (*currentUser)->loggedIn)
                viewMyProfile(*currentUser);
            else
                printf("Please login first.\n");

            break;

        case 5:
            if (*currentUser != NULL &&
                (*currentUser)->loggedIn)
                editMyProfile(*currentUser);
            else
                printf("Please login first.\n");

            break;

        case 6:
            searchUser();
            break;

        case 7:
            displayUsers();
            break;

        case 8:
            return;

        default:
            printf("Invalid choice.\n");
        }

    } while (choice != 8);
}


/* =========================================================
   FRIEND MENU
   ========================================================= */

void friendMenu(User *currentUser)
{
    int choice;

    if (currentUser == NULL ||
        !currentUser->loggedIn)
    {
        printf("\nPlease login first.\n");
        return;
    }

    do
    {
        printf("\n\n");
        printf("============================================\n");
        printf("                  ?? FRIENDS\n");
        printf("============================================\n");

        printf("1. Send Friend Request\n");
        printf("2. Accept / Reject Request\n");
        printf("3. Remove Friend\n");
        printf("4. View Friend List\n");
        printf("5. View Mutual Friends\n");
        printf("6. Friend Suggestions\n");
        printf("7. Back\n");

        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            sendFriendRequest(currentUser);
            break;

        case 2:
            receivedRequests(currentUser);
            break;

        case 3:
            removeFriend(currentUser);
            break;

        case 4:
            displayFriendList(currentUser);
            break;

        case 5:
            viewMutualFriends();
            break;

        case 6:
            friendSuggestions(currentUser);
            break;

        case 7:
            return;

        default:
            printf("Invalid choice.\n");
        }

    } while (choice != 7);
}


/* =========================================================
   POST MENU
   ========================================================= */

void postMenu(User *currentUser)
{
    int choice;

    if (currentUser == NULL ||
        !currentUser->loggedIn)
    {
        printf("\nPlease login first.\n");
        return;
    }

    do
    {
        printf("\n\n");
        printf("============================================\n");
        printf("                  ?? POSTS\n");
        printf("============================================\n");

        printf("1. Create Post\n");
        printf("2. Post Feed\n");
        printf("3. Like Post\n");
        printf("4. Comment on Post\n");
        printf("5. Delete Own Post\n");
        printf("6. Back\n");

        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            createPost(currentUser);
            savePosts();
            break;

        case 2:
            postFeed(currentUser);
            break;

        case 3:
            likeSpecificPost(currentUser);
            break;

        case 4:
            commentSpecificPost(currentUser);
            break;

        case 5:
            deleteOwnPost(currentUser);
            break;

        case 6:
            return;

        default:
            printf("Invalid choice.\n");
        }

    } while (choice != 6);
}


/* =========================================================
   NOTIFICATION MENU
   ========================================================= */

void notificationMenu(User *currentUser)
{
    if (currentUser == NULL ||
        !currentUser->loggedIn)
    {
        printf("\nPlease login first.\n");
        return;
    }

    viewNotifications(currentUser);
}


/* =========================================================
   LOAD ALL DATA
   ========================================================= */

void loadAllData()
{
    /*
       Users must be loaded first because
       friends, posts and comments depend on users.
    */

    loadUsers();

    loadFriends();

    loadRequests();

    loadPosts();

    loadLikes();

    loadComments();

    loadNotifications();
}


/* =========================================================
   SAVE ALL DATA
   ========================================================= */

void saveAllData()
{
    saveUsers();
    saveFriends();
    saveRequests();
    savePosts();
    saveLikes();
    saveComments();
    saveNotifications();
}


/* =========================================================
   MAIN
   ========================================================= */

int main()
{
    int choice;

    User *currentUser = NULL;

    /*
       Load old data when program starts.
    */
    loadAllData();

    printf("\n============================================\n");
    printf("     SOCIAL NETWORK SYSTEM STARTED\n");
    printf("============================================\n");

    do
    {
        printf("\n\n");
        printf("==================================================\n");
        printf("       ?? SOCIAL NETWORK MANAGEMENT SYSTEM\n");
        printf("==================================================\n");

        if (currentUser != NULL &&
            currentUser->loggedIn)
        {
            printf("Logged in: %s (ID: %d)\n",
                   currentUser->name,
                   currentUser->id);
        }
        else
        {
            printf("Status: Not Logged In\n");
        }

        printf("--------------------------------------------------\n");

        printf("1. ?? User / Profile\n");
        printf("2. ?? Friends\n");
        printf("3. ?? Posts\n");
        printf("4. ?? Notifications\n");
        printf("5. ?? Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            userMenu(&currentUser);
            break;

        case 2:
            friendMenu(currentUser);
            break;

        case 3:
            postMenu(currentUser);
            break;

        case 4:
            notificationMenu(currentUser);
            break;

        case 5:
            saveAllData();

            printf("\nAll data saved successfully.\n");
            printf("Thank you for using Social Network Management System!\n");

            break;

        default:
            printf("\nInvalid choice.\n");
        }

    } while (choice != 5);

    return 0;
}
