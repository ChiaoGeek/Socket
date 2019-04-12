a. Full Name:  Zhao Chang

b. StudentID:  8728868014

c. What you have done in the assignment.
    I know about the principle of TCP and UDP, and wrote this program and debugged it.

d. What your code files are and what each one of them does. (Please do not repeat the project description,
   just name your code files and briefly mention what they do).

	aws.cpp---------This file has two TCP servers, which serve client and monitor,  and one UDP server, which communicates with ServerA and ServerB .

	serverA.cpp-----Used to store the data.

	serverB.cpp-----Used to compute the results.

	client.cpp------Used to communicate with users.

	monitor.cpp-----Used to monitor all progress.

e. The format of all the messages exchanged.

    client to aws ---- write <BW> <LENGTH> <VELOCITY> <NOISEPOWER> or compute <LINK_ID> <SIZE> <SIGNALPOWER>
    aws to client --- res <TE>
    aws to serverA ---- write <BW> <LENGTH> <VELOCITY> <NOISEPOWER> or search <ID>
    aws to serverB ---- compute <SIZE> <SIGNALPOWER> <BW> <LENGTH> <VELOCITY> <NOISEPOWER>
    serverA to aws ---- id <ID> or res <BW> <LENGTH> <VELOCITY> <NOISEPOWER>
    serverB to aws ---- res <TE>
    monitor to aws ---- write <BW> <LENGTH> <VELOCITY> <NOISEPOWER> or compute <LINK_ID> <SIZE> <SIGNALPOWER> or res <TE> or res <BW> <LENGTH> <VELOCITY> <NOISEPOWER>

f. Any idiosyncrasy of your project. It should say under what conditions the project fails, if any.

g. Reused Code: Did you use code from anywhere for your project? If not, say so.
   If so, say what functions and where they're from. (Also identify this with a comment in the source code.)







