With some work, I made a true clean compilable working version of Syser Debugger 1.99.1900.1000;

Just open Project\Syser\Win32\Syser.sln solution with VS2022, compile and run (with or without debugger).
(can run only if executed from visual studio because of current (debug) directory specified in solution
 and source code is modified to use the current directory instead of executable directory);
 (Only Win32-DEBUG configuration is tested);

Majors changes to the original source code are annotated with "JLS" in comments.

all code compile with /permissive-, C++14.


original source :
https://github.com/yanfengwu-syser/syserdebugger


# SyserDebugger NG

Pay respect to the original author of syser debugger: [Yanfeng Wu](ori_author.md)

The open source project is as his last wish.



## Build

[Buidling method](Doc/build.md)
