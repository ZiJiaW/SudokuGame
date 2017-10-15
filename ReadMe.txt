A cooperating SE project with my partner FelixCaae.
It's a SudokuGame program.
可执行文件在BIN目录下：
sudoku.exe是命令行程序，支持下列指令：

-s file_path：从file_path中读取文件，将文件中的数独解出，写入另一个文件sudoku.txt中，注意两个文件的格式（包括下面的）均为：
x x x x x x x x x
x x x x x x x x x
......
即数独9*9，每行两数之间加空格，行末尾无回车，两个数独之间空一行；

-c N：生成N个不重复的数独终局至同目录下文件sudoku.txt中；

-n <number> -m <mode>：生成number个难度为mode的数独题目至同目录下文件sudoku.txt中，其中number范围为[1,10000]，mode为1,2,3，分别表示3种难度；

-n <number> -r <lower>~<upper> -u：-r与-u参数可选其一或其二，lower和upper表示生成的数独题目空的数目的上下限，范围为[20,55]，注意upper>=lower；-u参数指定其是否唯一解；

使用-n参数的命令各个参数顺序可调换；

制作的数独游戏在BIN目录下的SudokuGUI.rar中，下载下来解压后的文件中，打开SudokuGame.exe即可，注意不能随意删掉文件中的库文件或是移动SudokuGame.exe的位置；
