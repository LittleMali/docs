https://unix.stackexchange.com/questions/118811/why-cant-i-run-gui-apps-as-root-no-protocol-specified


Accessing the X server requires two things:

1. The $DISPLAY variable pointing to the correct display (usually :0)
2. Proper authentication information



打印授权位置
echo $XAUTHORITY
也可以通过
xauth info
