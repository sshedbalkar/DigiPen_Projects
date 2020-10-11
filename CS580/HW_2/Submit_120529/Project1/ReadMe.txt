What is Implemented:
A sequence of various acts done by 13 Tinys. 12 Tinys form 3 teams each consisiting of 4 Tinys. The 13th Tiny acts like a Announcer who controls these 3 teams. When the program runs all Tinys go to their position. Then they perform a relay race as directed by the announcer. Then they perform 2 acts of switching places.

Directions:
Just compile the project or run the exe, no specific directions.

Exxperience with working on this project:
The project has a learning curve associated with it. I spent like 4-5 hrs trying to understand the SML by looking at the code and going through the slides. The slides are really helpful. But at some places, they don't give specific directions as to how to use a particular function (ex: how to use OnCCMsg(). No clear instructions). The macros are pretty hard to understand. I didn't even try to understand them. The project is pretty straight forward for implementation IF you have a good idea of what you want to implement. And the tight requirements make it very hard to find an idea that meets all the requirements. I spent considerable amount of time just thinking about the idea but ultimately couldn't come up with anything interesting. I spent a total of about 18 hrs on the project.

10 features implemented:
1. Substates: File: SM_Runner_1.cpp; Line: 67
2. Global Message Response: File: SM_Runner_1.cpp; Line: 34
3. SendMsgDelayedToState: File: SM_Announcer; Line: 54
4. ChangeStateDelayed: File: SM_Runner_1.cpp; Line: 40
5. Data Passed in Msg: File: SM_Runner_1.cpp; Line: 286
6. OnCCMsg: File: SM_Runner_1.cpp; Line: 60
7. SendMsgBroadcastToList: File: SM_Announcer.cpp; Line: 47
8. OnTimeInSubstate: File: SM_Announcer.cpp; Line: 68
9. DeclareStateInt: File: SM_Runner_1.cpp; Line: 256
10. PopState: File: File: SM_Announcer.cpp; Line: 79

Extra Credits attempted:
Bug report: Using DeclareStatePointerVec3()/DeclareSubstatePointerVec3() gives runtime error: Accessing memory location 0x00000000