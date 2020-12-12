# MusicPlayer-using-C
Windows based Music Player using C!

The music player project is built using CodeBlocks. You can import it directly using the Cbp File.

If building seperately, do not forget to link the winmm dependency. It will not work otherwise. 
This works only on windows operating system as it uses the winmm api to play music.

The main code is based on a doubly linked list to navigate between playlists. Just add all the mp3 and wav files in the main directory and run the exe.
The music player should automatically recognize songs once they are placed  in the main directory.
