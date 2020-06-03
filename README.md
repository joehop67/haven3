# Haven3
### Because Electron is so last year

Welcome to the Haven3 GitHub Repository. Here you will find the source code for what is, at this point, my third attempt at creating a text editor that doesn't swallow RAM whole just to open text files.

## Rationale (Rationalization, perhaps?)
I have several but none are any more satisfying than the others. Really, I wanted an excuse to build an app in C++. And since I've written at least two other iterations of this bloody text editor (See my original Haven project), I figured I might as well give it another go. There's not much structure to be seen here, and I'm still getting familiar with all of the... *ahem* "features" of C++, especially cross platform and GUI.

## Design Goals
Aside from me working on... Just about whatever I want in here as I find myself and my interests drifting from one system to another as I build this out, the **core** design tenant is:
* **LOW MEMORY USAGE ALWAY**

In my dayjob it's usually preferential to use a modern GUI text editor (ala VSCode, the bane of my existence). The problem I have had with these since I first became accostomed to using one is the simple fact that the most popular editors are all built in some flavor of JavaScript. This is jolly well good when it comes most, small desktop apps, but in a resource intensive application like VSCode, the RAM usage and subsequent system slowdown is absolutely ridiculous sometimes. I loathe Electron and the seemingly wanton disregard everyone has for adequate memory managment, so I'm just gonna write my own. (Also, please don't get me wrong, I love VSCode and Electron has transformed the development landscape forever. However, we can't deny that they are huge resource hogs. Annoyingly slow in my opinion. I'm still going to use VSCode, but if I can write something that works how I want then... All I see is win).

Anyway, I have absolutely no idea why you're still reading this. Let's be honest, you're lucky you even got this in depth of a read me. /s

Feel free to do... whatever with this. If you have any comments or criticisms of my code please feel free to submit an issue. I'm only doing this to improve me C skills after all, so that'd definitely be great.