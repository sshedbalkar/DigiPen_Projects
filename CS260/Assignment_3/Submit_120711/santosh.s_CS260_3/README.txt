Name:		Santosh Shedbalkar
Course:		CS260
Assignment:	3 (File Sharing)
-------------------------------------------

1. The solution has two projects, namely, Client and Server
2. Both of these builds accept the path of the config.txt file as command line argument. If you don't pass one then it assumed that the config file is placed where the VS2010 solution file exists. A sample config.txt file is provided in the same folder as the VS2010 solution file.
3. First, run the Server.exe program and then run the Client.exe program
4. Type '/show' on the client's console to see the list of available downloads.
5. Type '/dl <ID of the file> to download the file represented by the ID. (Each file is given an ID which is displayed to the user when he types '/show'. I did it this way because it is far easier to type compared to typing out the entire name of the file)
6. Note that I have implemented a non blocking console input system. And for that I had to implement the functionalities of an editor myself. So the Console input behavior is not quite as you'd expect. You can only type. There is no backspace or arrow keys or any other functionality. I tried to implement the backspace but for some reason it is not working. If you type something wrong then dont press backspace, just press Enter. If the comand is wrong, the system will just tell you that it's an invalid command.

Extra credits implemented:
1. Ability to receive files from multiple hosts simultaneously i.e. Host A can receive a file from both Host B and Host C at the same time. 