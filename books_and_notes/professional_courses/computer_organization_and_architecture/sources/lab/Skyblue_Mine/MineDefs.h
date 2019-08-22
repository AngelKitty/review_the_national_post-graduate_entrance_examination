#ifndef _MINEDEF
#define _MINEDEF
// regedit defines
#define DEFAULT_LEVEL		0// for first run the default level will be primary(0)
#define DEFAULT_RECORD		999
#define DEFAULT_HOLDER		_T("匿名")
#define DEFAULT_MARKFUL		1
#define DEFAULT_COLORFUL	1
#define DEFAULT_SOUNDFUL	0
#define	PRIMARY_XNUM		9
#define PRIMARY_YNUM		9
#define PRIMARY_MINENUM		10
#define SECONDRY_XNUM		16
#define SECONDRY_YNUM		16
#define SECONDRY_MINENUM	40
#define ADVANCE_XNUM		30
#define ADVANCE_YNUM		16
#define ADVANCE_MINENUM		99

#define GAME_SETTING		_T("GameSetting")
#define GAME_LEVEL			_T("CurrentLevel")	// 0:Primary1:Second2:Advance
#define GAME_MARKFUL		_T("Markful")
#define GAME_COLORFUL		_T("Colorful")
#define GAME_SOUNDFUL		_T("Soundful")
#define PRIMARY_RECORD		_T("PrimaryRecord")
#define PRIMARY_HOLDER		_T("PrimaryHolder")
#define SECOND_RECORD		_T("SecondRecord")
#define SECOND_HOLDER		_T("SecondHolder")
#define ADVANCE_RECORD		_T("AdvanceRecord")
#define ADVANCE_HOLDER		_T("AdvanceHolder")
#define CUSTOM_XNUM			_T("CustomXNum")	// X coordinate mine rect numbers
#define CUSTOM_YNUM			_T("CustomYNum")	// Y coordinate mine rect numbers
#define CUSTOM_MINENUM		_T("CustomMineNum")	// Custom total mine numbers (in default: Primary=10, Second=40, Advance=99)

// 窗口宽度相关定义
#define	DEFAULT_FRAME_X		6          //窗口X方向宽
#define DEFAULT_FRAME_Y		52

#define LINE_WIDTH_0		3          //线边0的宽度					
#define LINE_WIDTH_1		2          //线边1的宽度

#define SIDE_WIDTH_0		6          //边0的宽度
#define SIDE_WIDTH_1		5          //边1的宽度

#define SHELL_S_H			37         //小外壳的高度
#define SHELL_S_START_X		9          //小外壳的x坐标始发点
#define SHELL_S_START_Y		9          //小外壳的y坐标始发点
#define SHELL_L_START_X		9          //大外壳的x坐标始发点
#define SHELL_L_START_Y		52         //大外壳的y坐标始发点

#define MINEAREA_FRAME_X	12                 
#define MINEAREA_FRAME_Y	55

// 雷方块定义
#define MINE_WIDTH			16         //雷方块的大小(宽度为16的位图)
#define MINE_HEIGHT			16
#define MINE_AREA_LEFT		12                
#define MINE_AREA_TOP		55			

#define STATE_NORMAL		0     //正常
#define STATE_FLAG			1    //标志为雷
#define STATE_DICEY			2    //未知状态0
#define STATE_BLAST			3    //爆炸状态
#define STATE_ERROR			4   //错误状态
#define STATE_MINE			5   //雷状态
#define STATE_DICEY_DOWN	6  //未知状态1
#define STATE_NUM8			7  //周围有8雷
#define STATE_NUM7			8 
#define STATE_NUM6			9 
#define STATE_NUM5			10
#define STATE_NUM4			11
#define STATE_NUM3			12
#define STATE_NUM2			13
#define STATE_NUM1			14
#define STATE_EMPTY			15  //无雷

#define ATTRIB_EMPTY		0
#define ATTRIB_MINE			1

// color defines
#define COLOR_BLACK			RGB(0, 0, 0)
#define COLOR_DARK_GRAY		RGB(128, 128, 128)
#define COLOR_GRAY			RGB(192, 192, 192)
#define COLOR_WHITE			RGB(255, 255, 255)

// 游戏状态
#define GS_WAIT				0
#define GS_RUN				1
#define GS_DEAD				2
#define GS_VICTORY			3

// 游戏级别
#define LEVEL_PRIMARY			0
#define LEVEL_SECONDRY			1
#define LEVEL_ADVANCE			2
#define LEVEL_CUSTOM			3

// 笑脸按钮矩形图像 状态
#define BUTTON_DOWN				0
#define BUTTON_VICTORY			1
#define BUTTON_DEAD				2
#define BUTTON_CLICK			3
#define BUTTON_NORMAL			4

// 失败去的原因
#define DC_STRIKE_MINE		0
#define DC_ERROR_MINE		1

#endif