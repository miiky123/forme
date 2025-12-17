# Project_MKH
https://github.com/miiky123/Project_MKH.git

This application manage storage effectivly by compressing files using RLE encoding, and take handle user queries; add, get and search.
add: 
the user would add a file, which has a name, and text content. this function is activated from the terminal, insert to the terminal in
this format: add [file name] [text]

search:
The user can search for patterns within files stored in the app. If found, the app would return all file names that contain that pattern.
this function is activated from the terminal, insert to the terminal in this format: search [file content]

get:
The user would recieve the content of the requested file (after decompressed). 
this function is activated from the terminal, insert to the terminal in this format: get [file name]

to execute to app:
This app is Docker Based.
To build the docker container follow these following steps:

 1. Build the Docker image:
docker build -t NAME .

<img width="1258" height="186" alt="image" src="https://github.com/user-attachments/assets/699cce9a-2bdc-4d5c-96fc-1b3c531bff5a" />

2. Running the main application:
  docker compose up --build server


CPP client:
cd clients after that
g++ -std=c++17 client.cpp -o client ----- and then sepratly :  
./client 127.0.0.1 9090

<img width="1600" height="409" alt="image" src="https://github.com/user-attachments/assets/551197f2-ecd5-45d4-908a-8f52373fc745" />

Python client:
cd clients after that
 python3 Pclient.py 127.0.0.1 9090

 <img width="1600" height="422" alt="image" src="https://github.com/user-attachments/assets/51c1cf63-7abc-486d-a5f0-8188bb04e8dc" />
 
<img width="1267" height="402" alt="image" src="https://github.com/user-attachments/assets/54d4d3b4-5a06-429b-908e-3f06e2383d4d" />

3. Running the test:
docker compose up --build tests
File Storage Directory:
/usr/src/Project_MKH/data


Answers:

1. Yes, app used directly the names of the command if the user inserted the arguments corretly. We changed it so each command would check the argument by it self, and now app is disconnect from checking the user arguemnt.

2.Yes, we needed to change the code to support diffrenet streams, we did it by creating an abstract class that would function as our stream interface, and added to the command as arugemnt the interface of stream. and to implemnt diffrent stream we would create a class that would inhernt the abstract stream class.

3. No, we only extened our code. We just added output based on sucess or failed. in previous exercies we already checked if the input is valid.

4. Yes, we changed the code so the logic of the code will be now server - client based so future code would be based on this - as exepected of this drive application.  and if we need to extend our code in the future it would be based on this.













