
#### 2.2.3 安装使用Linux实验环境（适合希望自己安装Linux系统的同学）

这里我们主要以Ubuntu Linux 14.04（64 bit）作为整个实验的系统软件环境。首先我们需要安装Ubuntu Linux 14.04。


**使用Linux**
在实验过程中，我们需要了解基于命令行方式的编译、调试、运行操作系统的实验方法。为此，需要了解基本的Linux命令行使用。
 
***命令模式的基本结构和概念***
Ubuntu是图形界面友好和易操作的linux发行版，但有时只需执行几条简单的指令就可以完成繁琐的鼠标点击才能完成的操作。linux的命令行操作模式功能可以实现你需要的所有操作。简单的说，命令行就是基于字符命令的用户界面，也被称为文本操作模式。绝大多数情况下， 用户通过输入一行或多行命令直接与计算机互动，来实现对计算机的操作。
 
***如何进入命令模式***
假设使用默认的图形界面为GNOME的任意版本Ubuntu Linux。点击GNOME菜单->附件->终端，就可以启动名为gnome-terminal程序，从而可以在此软件界面中进行命令行操作。
打开gnome-terminal程序后你首先可能会注意到类似下面的界面：

	chy@chyhome-PC:~$ ls
	file1.txt  file2.txt file3.txt tools

你所看到的这些被称为命令终端提示符，它表示计算机已就绪，正在等待着用户输入操作指令。以我的屏幕画面为例，“chy"是当前所登录的用户名，“laptop”是这台计算机的主机名，“~”表示当前目录。此时输入任何指令按回车之后该指令将会提交到计算机运行，比如你可以输入命令：ls 再按下回车：

	ls [ENTER]

*注意*：[ENTER]是指输入完ls后按下回车键，而不是叫你输入这个单词，ls这个命令将会列出你当前所在目录里的所有文件和子目录列表。 

下面介绍bash shell程序的基本使用方法，它是ubuntu缺省的外壳程序。

***常用指令***

(1) 查询文件列表：(ls)

	chy@chyhome-PC:~$ ls
	file1.txt  file2.txt file3.txt tools

ls命令默认状态下将按首字母升序列出你当前文件夹下面的所有内容，但这样直接运行所得到的信息也是比较少的，通常它可以结合以下这些参数运行以查询更多的信息： 

	ls /		# 将列出根目录'/'下的文件清单.如果给定一个参数，则命令行会把该参数当作命令行的工作目录。换句话说，命令行不再以当前目录为工作目录。 
	ls -l 		# 将给你列出一个更详细的文件清单. 
	ls -a		# 将列出包括隐藏文件(以.开头的文件)在内的所有文
件. 	]ls -h		# 将以KB/MB/GB的形式给出文件大小,而不是以纯粹的Bytes. 
 
(2) 查询当前所在目录：(pwd)

	chy@chyhome-PC:~$ pwd
	/home/chy
 
(3) 进入其他目录：(cd) 
	chy@chyhome-PC:~$ pwd
	/home/chy
	chy@chyhome-PC:~$ cd /root/
	chy@chyhome-PC:~$ pwd
	/root

上面例子中，当前目录原来是/home/chy,执行cd /root/之后再运行pwd可以发现，当前目录已经改为/root了。
 
(4) 在屏幕上输出字符： (echo)

	chy@chyhome-PC:~$ echo "Hello World"
	Hello World

这是一个很有用的命令，它可以在屏幕上输入你指定的参数(""号中的内容)，当然这里举的这个例子中它没有多大的实际意义，但随着你对LINUX指令的不断深入，就会发现它的价值所在。 
 
(5) 显示文件内容：cat

	chy@chyhome-PC:~$ cat file1.txt
	Roses are red.
	Violets are blue,
	and you have the bird-flue!

也可以使用less或more来显示比较大的文本文件内容。
 
(6) 复制文件： cp

	chy@chyhome-PC:~$ cp file1.txt file1_copy.txt
	chy@chyhome-PC:~$ cat file1_copy.txt
	Roses are red.
	Violets are blue,
	and you have the bird-flue!
 
(7) 移动文件：mv

	chy@chyhome-PC:~$ ls
	file1.txt
	file2.txt
	chy@chyhome-PC:~$ mv file1.txt new_file.txt
	chy@chyhome-PC:~$ ls
	file2.txt
	new_file.txt

*注意*：在命令操作时系统基本上不会给你什么提示，当然，绝大多数的命令可以通过加上一个参数-v来要求系统给出执行命令的反馈信息； 

	chy@chyhome-PC:~$ mv -v file1.txt new_file.txt
	`file1.txt' -> `new_file.txt'
 
(8) 建立一个空文本文件：touch

	chy@chyhome-PC:~$ ls
	file1.txt
	chy@chyhome-PC:~$ touch tempfile.txt
	chy@chyhome-PC:~$ ls
	file1.txt
	tempfile.txt
 
(9) 建立一个目录：mkdir

	chy@chyhome-PC:~$ ls
	file1.txt
	tempfile.txt
	chy@chyhome-PC:~$ mkdir test_dir
	chy@chyhome-PC:~$ ls
	file1.txt
	tempfile.txt
	test_dir
 
(10) 删除文件/目录：rm

	chy@chyhome-PC:~$ ls -p
	file1.txt
	tempfile.txt
	test_dir/
	chy@chyhome-PC:~$ rm -i tempfile.txt
	rm: remove regular empty file `test.txt'? y
	chy@chyhome-PC:~$ ls -p
	file1.txt
	test_dir/
	chy@chyhome-PC:~$ rm test_dir
	rm: cannot remove `test_dir': Is a directory
	chy@chyhome-PC:~$ rm -R test_dir
	chy@chyhome-PC:~$ ls -p
	file1.txt
 
在上面的操作：首先我们通过ls命令查询可知当前目下有两个文件和一个文件夹；

	[1] 你可以用参数 -p来让系统显示某一项的类型，比如是文件/文件夹/快捷链接等等；
	[2] 接下来我们用rm -i尝试删除文件，-i参数是让系统在执行删除操作前输出一条确认提示；i(interactive)也就是交互性的意思； 
	[3] 当我们尝试用上面的命令去删除一个文件夹时会得到错误的提示，因为删除文件夹必须使用-R(recursive,循环）参数

特别提示：在使用命令操作时，系统假设你很明确自己在做什么，它不会给你太多的提示，比如你执行rm -Rf /，它将会删除你硬盘上所有的东西，并且不会给你任何提示，所以，尽量在使用命令时加上-i的参数，以让系统在执行前进行一次确认，防止你干一些蠢事。如 果你觉得每次都要输入-i太麻烦，你可以执行以下的命令，让－i成为默认参数： 

	alias rm='rm -i'
 
(11) 查询当前进程：ps

	chy@chyhome-PC:~$ ps
	PID TTY          TIME CMD
	21071 pts/1    00:00:00 bash
	22378 pts/1    00:00:00 ps

这条命令会例出你所启动的所有进程； 

	ps -a		#可以例出系统当前运行的所有进程，包括由其他用户启动的进程； 
	ps auxww	#是一条相当人性化的命令，它会例出除一些很特殊进程以外的所有进程，并会以一个高可读的形式显示结果，每一个进程都会有较为详细的解释； 
 
基本命令的介绍就到此为止，你可以访问网络得到更加详细的Linux命令介绍。
 
***控制流程***
(1) 输入/输出

input用来读取你通过键盘（或其他标准输入设备）输入的信息，output用于在屏幕（或其他标准输出设备）上输出你指定的输出内容.另外还有一些标准的出错提示也是通过这个命令来实现的。通常在遇到操作错误时，系统会自动调用这个命令来输出标准错误提示； 

我们能重定向命令中产生的输入和输出流的位置。

(2) 重定向

如果你想把命令产生的输出流指向一个文件而不是（默认的）终端，你可以使用如下的语句：

	chy@chyhome-PC:~$ ls >file4.txt
	chy@chyhome-PC:~$ cat file4.txt
	file1.txt  file2.txt file3.txt
以上例子将创建文件file4.txt如果file4.txt不存在的话。注意：如果file4.txt已经存在，那么上面的命令将复盖文件的内容。如果你想将内容添加到已存在的文件内容的最后，那你可以用下面这个语句： 

	command >> filename 

示例: 

	chy@chyhome-PC:~$ ls >> file4.txt
	chy@chyhome-PC:~$ cat file4.txt
	file1.txt  file2.txt file3.txt
	file1.txt  file2.txt file3.txt file4.txt
 
在这个例子中，你会发现原有的文件中添加了新的内容。接下来我们会见到另一种重定向方式：我们将把一个文件的内容作为将要执行的命令的输入。以下是这个语句： 

	command < filename 

示例: 

	chy@chyhome-PC:~$ cat > file5.txt
	a3.txt
	a2.txt
	file2.txt
	file1.txt
	<Ctrl-D>  # 这表示敲入Ctrl+D键
	chy@chyhome-PC:~$ sort < file5.txt
	a2.txt
	a3.txt
	file1.txt
	file2.txt
 
(3) 管道

Linux的强大之处在于它能把几个简单的命令联合成为复杂的功能，通过键盘上的管道符号'|' 完成。现在，我们来排序上面的"grep"命令： 

	grep -i command < myfile | sort > result.text 

搜索 myfile 中的命令，将输出分类并写入分类文件到 result.text 。 有时候用ls列出很多命令的时候很不方便 这时“｜”就充分利用到了 ls -l | less 慢慢看吧. 
 
(4) 后台进程

CLI 不是系统的串行接口。您可以在执行其他命令时给出系统命令。要启动一个进程到后台，追加一个“&”到命令后面。 

	sleep 60 &
	ls

睡眠命令在后台运行，您依然可以与计算机交互。除了不同步启动命令以外，最好把 '&' 理解成 ';'。 

如果您有一个命令将占用很多时间，您想把它放入后台运行，也很简单。只要在命令运行时按下ctrl-z，它就会停止。然后键入 bg使其转入后台。fg  命令可使其转回前台。 

	sleep 60
	<ctrl-z> # 这表示敲入Ctrl+Z键
	bg
	fg

最后，您可以使用 ctrl-c 来杀死一个前台进程。 

***环境变量***

特殊变量。PATH, PS1, ... 

(1) 不显示中文

可通过执行如下命令避免显示乱码中文。在一个shell中，执行：

	export LANG=””
 
这样在这个shell中，output信息缺省时英文。

***获得软件包***

(1) 命令行获取软件包

Ubuntu 下可以使用 apt-get 命令，apt-get 是一条 Linux 命令行命令，适用于 deb 包管理式的操作系统，主要用于自动从互联网软件库中搜索、安装、升级以及卸载软件或者操作系统。一般需要 root 执行权限，所以一般跟随 sudo 命令，如：

	sudo apt-get install gcc [ENTER]

常见的以及常用的 apt 命令有：
	
	apt-get install <package>
		下载 <package> 以及所依赖的软件包，同时进行软件包的安装或者升级。
	apt-get remove <package>
		移除 <package> 以及所有依赖的软件包。
	apt-cache search <pattern>
		搜索满足 <pattern> 的软件包。
	apt-cache show/showpkg <package>
		显示软件包 <package> 的完整描述。
例如：

	chy@chyhome-PC:~$apt-cache search gcc
	gcc-4.8 - The GNU C compiler
	gcc-4.8-base - The GNU Compiler Collection (base package)
	gcc-4.8-doc - Documentation for the GNU compilers (gcc, gobjc, g++)
	gcc-4.8-multilib - The GNU C compiler (multilib files)
	gcc-4.8-source - Source of the GNU Compiler Collection
	gcc-4.8-locales - The GNU C compiler (native language support files)
	chy@chyhome-PC:~$
(2) 图形界面软件包获取
新立得软件包管理器，是 Ubuntu 下面管理软件包得图形界面程序，相当于命令行中得 apt 命令。进入方法可以是 

	菜单栏 > 系统管理 > 新立得软件包管理器
	(System > Administration > Synaptic Package Manager)

使用更新管理器可以通过标记选择适当的软件包进行更新操作。

(3) 配置升级源

Ubuntu的软件包获取依赖升级源，可以通过修改 “/etc/apt/sources.list” 文件来修改升级源（需要 root 权限）；或者修改新立得软件包管理器中 “设置 > 软件库”。

***查找帮助文件***
Ubuntu 下提供 man 命令以完成帮助手册得查询。man 是 manual 的缩写，通过 man 命令可以对 Linux 下常用命令、安装软件、以及C语言常用函数等进行查询，获得相关帮助。

例如：

	cchy@chyhome-PC:~$man printf
	PRINTF(1)                 BSD General Commands Manual                	PRINTF(1)
	 
	NAME
	     printf -- formatted output
	 
	SYNOPSIS
	     printf format [arguments ...]
	 
	DESCRIPTION
	     The printf utility formats and prints its arguments, after the first, under control of the format. The format is a character string which contains three types of objects: plain characters, which are simply copied to standard output, character escape sequences which are converted and copied to the standard output, and format specifications, each of which causes ...
	           ...
	     The characters and their meanings are as follows:
	           \e      Write an <escape> character.
	           \a      Write a <bell> character.
	           ...
 
通常可能会用到的帮助文件例如：

	gcc-doc cpp-doc glibc-doc

上述帮助文件可以通过 apt-get 命令或者软件包管理器获得。获得以后可以通过 man 命令进行命令或者参数查询。
 
