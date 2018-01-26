# Team Waddles presenting ... Clubby

**Team:**

Xing Tao Shi, Herman Lin, James Ko

Period 4

**Overview:**

Our project is essentially a chat program. There are three seperate chat rooms that the user can move between and anything sent to the room will be displayed to all clients in the room. Users can also type "/all" before their message in order to send it to everyone, regardless of room.

**Instructions:**

First, clone the repo locall and then run the command:

```
$ make
```
Then use tmux or open new windows in your terminal and in one run:

'''
$ ./server
'''

For however many clients you want, run:

'''
$ ./client
'''

Afterwards, you can type into your server to broadcast a message to all the clients. You can also type in the clients to talk to the current room (everyone is defaultly placed in the same one). In the client, you can type "left" or "right" to move rooms. You can also preceed ur message with "/all" to send it to all users regardless of room. To exit the program, type "exit" into the server.

**Known Bugs**
- sigint stops the program despite having a sighandler which does nothing
- unable to disconnect individual clients, must exit on server to disconnect everyone
