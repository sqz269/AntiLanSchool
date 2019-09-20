# AntiLanSchool

A program that prevents lanschool from partially or completely controlling the computer  

## Features  

- Terminates Process that controls the computer (student.exe)  
- Suspend/Resumes stduent.exe  
- Removes the extension that controls chrome to remove the website restrictions enforced  
- Prevents lanschool extension from reinstalling also stops other Extension from installing ~~(Work in Progress)~~  
- Completely Removes policies enforced on chrome browser (Work in progress)  
- Only Remove policy that forcibly installs an extension (Work in progress)
- Only Remove policy that forbids an extension from installing (Work in progress)

## How to use

1. Download [PSTools](https://docs.microsoft.com/en-us/sysinternals/downloads/psexec)  
2. Extract PSTools and keep PsExec.exe or PsExec64.exe (depends on the CPU architecture, but PsExec.exe will work on both 64/32bit systems)  
3. Start the program (Must start as administrator)  
4. Enter the PsExec path you download in step 2  
5. Choose options  

### WARNING: BOTH PSEXEC AND THE PROGRAM ITSELF MUST BE ON A LOCAL DRIVE, NOT ON A NETWORK DRIVE

## Contributing
Any contributions are welcome

## Extra

- Use carefully, I am not responsible for any consequences caused by this software
- You can submite a feature request (Include details) on issues

### Might do

- Add Options to Launch Regedit/TaskMgr
- Add Options to remove lanschool extension w/o requiring Administrator privilege
- Automatically download PsExec
- Interestingly LanSchool seems to also include a keylogger according to [this](https://forums.hak5.org/topic/15087-lanschool-keylogger-vulnerability/) source might also do a feature to disable the keylogger
