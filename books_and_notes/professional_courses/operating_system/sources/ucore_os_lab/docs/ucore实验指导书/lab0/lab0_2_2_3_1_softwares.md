
##### 2.2.3.1 实验中可能使用的软件

***编辑器***

(1) Ubuntu 下自带的编辑器可以作为代码编辑的工具。例如 gedit 是 gnome 桌面环境下兼容UTF-8的文本编辑器。它十分的简单易用，有良好的语法高亮，对中文支持很好。通常可以通过双击或者命令行打开目标文件进行编辑。


(2) Vim 编辑器：Vim是一款极方便的文本编辑软件，是UNIX下的同类型软件VI的改进版本。Vim经常被看作是“专门为程序员打造的文本编辑器”，功能强大且方便使用，便于进行程序开发。
Ubuntu 下默认安装的 vi 版本较低，功能较弱，建议在系统内安装或者升级到最新版本的 Vim。

[1]关于Vim的常用命令以及使用，可以通过网络进行查找。

[2]配置文件：Vim 的使用需要配置文件进行设置，例如：

	set nocompatible
	set encoding=utf-8
	set fileencodings=utf-8,chinese
	set tabstop=4
	set cindent shiftwidth=4
	set backspace=indent,eol,start
	autocmd Filetype c set omnifunc=ccomplete#Complete
	autocmd Filetype cpp set omnifunc=cppcomplete#Complete
	set incsearch
	set number
	set display=lastline
	set ignorecase
	syntax on
	set nobackup
	set ruler
	set showcmd
	set smartindent
	set hlsearch
	set cmdheight=1
	set laststatus=2
	set shortmess=atI
	set formatoptions=tcrqn
	set autoindent  

可以将上述配置文件保存到：

	~/.vimrc

注意：.vimrc 默认情况下隐藏不可见，可以在命令行中通过 “ls -a” 命令进行查看。如果 '~' 目录下不存在该文件，可以手动创建。修改该文件以后，重启 Vim 可以使配置生效。

***exuberant-ctags***
exuberant-ctags 可以为程序语言对象生成索引，其结果能够被一个文本编辑器或者其他工具简捷迅速的定位。支持的编辑器有 Vim、Emacs 等。
实验中，可以使用命令：

	ctags -h=.h.c.S -R
默认的生成文件为 tags (可以通过 -f 来指定)，在相同路径下使用 Vim 可以使用改索引文件，例如:

	使用 “ctrl + ]” 可以跳转到相应的声明或者定义处，使用 “ctrl + t” 返回（查询堆栈）等。

提示：习惯GUI方式的同学，可采用图形界面的understand、source insight等软件。
***diff & patch***

diff 为 Linux 命令，用于比较文本或者文件夹差异，可以通过 man 来查询其功能以及参数的使用。使用 patch 命令可以对文件或者文件夹应用修改。

例如实验中可能会在 proj_b 中应用前一个实验proj_a 中对文件进行的修改，可以使用如下命令：

	diff -r -u -P proj_a_original proj_a_mine > diff.patch
	cd proj_b
	patch -p1 -u < ../diff.patch

注意：proj_a_original 指 proj_a 的源文件，即未经修改的源码包，proj_a_mine 是修改后的代码包。第一条命令是递归的比较文件夹差异，并将结果重定向输出到 diff.patch 文件中；第三条命令是将 proj_a 的修改应用到 proj_b 文件夹中的代码中。

提示：习惯GUI方式的同学，可采用图形界面的meld、kdiff3、UltraCompare等软件。
