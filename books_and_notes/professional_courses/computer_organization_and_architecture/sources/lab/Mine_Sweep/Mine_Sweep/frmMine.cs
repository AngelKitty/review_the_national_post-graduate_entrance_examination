using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace winMine
{
    public partial class frmMine : Form
    {
        Brick[,] bricks;
        public const string flag = "";
        public const string explosive = "";
        public const string plug = "";
        public static int primaryX = 5,primaryY = 5;
        public static int middleX = 10, middleY = 10;
        public static int highX = 20, highY = 18;
        public static int mineX = 5, mineY = 5;
        public static bool plug_in = false;
        class Brick : Button
        {
            public bool isMine;
            public int x, y;

            public Brick(int in_x, int in_y, bool in_isMine = false)
            {
                x = in_x;
                y = in_y;
                isMine = in_isMine;
            }

            public bool unCovered()
            {
                if (Text == flag || Text == "" || Text == plug)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            //基础的画方块
            public void draw(string str, string fontName, int fontSize, Color colFore, Color colBack)
            {
                Text = str;
                Font = new System.Drawing.Font(fontName, fontSize);
                ForeColor = colFore;
                BackColor = colBack;
            }

            //普通方块
            public void drawNormal()
            {
                draw("", "Segoe UI", 14, Color.Black, Color.LightGray);
            }
            //陷下去的方块
            public void drawBase()
            {
                draw("", "Segoe UI", 14, Color.Black, Color.Snow);
            }
            //画雷
            public void drawMine()
            {
                draw(plug, "Wingdings", 14, Color.Black, Color.LightGray);
            }
            //爆炸
            public void drawExplosive()
            {
                draw(explosive, "Wingdings", 14, Color.Black, Color.Red);
            }
            //画旗子
            public void drawFlag()
            {
                draw(flag, "Wingdings", 14, Color.Black, Color.Yellow);
            }
            //画不同颜色的数字
            public void drawColor(int num)
            {
                Color currColor = Color.Black;
                switch (num)
                {
                    case 0:
                        currColor = Color.Snow;
                        break;
                    case 1:
                        currColor = Color.Blue;
                        break;
                    case 2:
                        currColor = Color.Green;
                        break;
                    case 3:
                        currColor = Color.Red;
                        break;
                    case 4:
                        currColor = Color.DarkBlue;
                        break;
                    case 5:
                        currColor = Color.Brown;
                        break;
                    case 6:
                        currColor = Color.LightBlue;
                        break;
                    case 7:
                        currColor = Color.Blue;
                        break;
                    case 8:
                        currColor = Color.Gray;
                        break;
                    default:
                        break;
                }
                draw(num.ToString(), "Seoge UI bold", 16, currColor, Color.Snow);
            }

        }

        public frmMine()
        {
            InitializeComponent();
            //this.Size = new System.Drawing.Size(400,450);
            mineX = primaryX;
            mineY = primaryY;
            toolStripMenuItem1.DropDownItems[0].Text = "初级 *";
            startNewGame();
        }

        void startNewGame()
        {
            this.Size = new System.Drawing.Size(80 + 32 * mineX, 128 + 32 * mineY);
            this.Controls.Clear();
            int upMargin = 64;
            int leftMargin = 32;
            int dx = 32, dy = 32;
            Random rd = new Random();
            bricks = new Brick[mineX, mineY];
            for (int i = 0; i < mineX; i++)
            {
                for (int j = 0; j < mineY; j++)
                {
                    Brick btn = new Brick(i, j);

                    //创建雷
                    if (rd.Next() % 6 == 0)
                    {
                        btn.isMine = true;
                        if (plug_in)
                        {
                         btn.drawMine();
                        }
                    }

                    btn.Location = new Point(leftMargin + dx * i, upMargin + j * dy);
                    btn.Size = new System.Drawing.Size(dx, dy);
                    btn.BackColor = Color.LightGray;
                    //btn.Text = i.ToString() + "," +j.ToString();
                    btn.MouseClick += btn_MouseClick;
                    btn.MouseUp += btn_MouseUp;
                    this.Controls.Add(btn);
                    bricks[i, j] = btn;
                    this.Controls.Add(menuStrip1);
                }
            }
            Button resetButton = new Button();
            resetButton.Size = new System.Drawing.Size(32, 32);
            resetButton.Location = new Point(Size.Width / 2 - 32, 32);
            resetButton.BackColor = Color.LightGray;
            resetButton.Text = "";
            resetButton.Font = new System.Drawing.Font("Wingdings",14);
            this.Controls.Add(resetButton);
            resetButton.MouseClick += resetButton_MouseClick;
        }

        bool isWin()
        {
            //统计雷数
            int mineCount = 0;
            //没打开的
            int unMineCount = 0;
            //遍历所有的格子
            foreach (Brick brk in bricks)
            {
                if (brk.isMine)
                {
                    mineCount++;
                }
                if (brk.unCovered())
                {
                    unMineCount++;
                }
            }
            if (mineCount == unMineCount)
            {
                return true;
            }
            else

                return false;
        }

        void resetButton_MouseClick(object sender, MouseEventArgs e)
        {
            startNewGame();
            return;
        }

        void btn_MouseUp(object sender, MouseEventArgs e)
        {
            Brick currBrick = (Brick)sender;
            if (e.Button == MouseButtons.Right)
            {
                if (currBrick.Text == flag)
                {
                    currBrick.drawNormal();
                }
                else
                {
                    currBrick.drawFlag();
                }
            }
        }

        void diagBrick(Brick currBrick)
        {
            //计算雷的数目
            int mineCount = 0;
            for (int i = -1; i <= 1; i++)
            {
                for (int j = -1; j <= 1; j++)
                {
                    //检索周围八个格子的雷的数目
                    int currX = currBrick.x + i;
                    int currY = currBrick.y + j;
                    if (currX >= 0 && currX < mineX && currY >= 0 && currY < mineY)
                    {
                        if (bricks[currX, currY].isMine)
                        {
                            mineCount++;
                        }
                    }
                }
            }
            currBrick.drawColor(mineCount);
            //判断是否是空即是mineCount = 0
            if (mineCount == 0)
            {
                //把周围的八个位置的格子都找一遍
                for (int i = -1; i <= 1; i++)
                {
                    for (int j = -1; j <= 1; j++)
                    {
                        //检索周围八个格子的雷的数目
                        int currX = currBrick.x + i;
                        int currY = currBrick.y + j;
                        if (currX >= 0 && currX < mineX && currY >= 0 && currY < mineY)
                        {
                            if (!bricks[currX, currY].isMine && bricks[currX, currY].Text == "")
                            {
                                //递归调用
                                diagBrick(bricks[currX, currY]);
                            }
                        }
                    }
                }
            }
        }

        void btn_MouseClick(object sender, MouseEventArgs e)
        {
            Brick currBrick = (Brick)sender;
            if (e.Button == MouseButtons.Left)
            {
                if (currBrick.Text == flag)
                {
                    return;
                }
                //判断是否是雷
                if (currBrick.isMine)
                {
                    currBrick.drawExplosive();
                    MessageBox.Show(" Boom! ","Information");
                    startNewGame();
                }
                else
                {
                    //挖雷
                    diagBrick(currBrick);
                    if (isWin())
                    {
                        MessageBox.Show("Bingo, you Win", "Information");
                        startNewGame();
                        return;
                    }

                }
            }

        }

        private void 初级ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            mineX = primaryX; mineY = primaryY;
            toolStripMenuItem1.DropDownItems[0].Text = "初级 *";
            toolStripMenuItem1.DropDownItems[1].Text = "中级";
            toolStripMenuItem1.DropDownItems[2].Text = "高级";
            startNewGame();
        }

        private void 中级ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            mineX = middleX; mineY = middleY;
            toolStripMenuItem1.DropDownItems[0].Text = "初级";
            toolStripMenuItem1.DropDownItems[1].Text = "中级 *";
            toolStripMenuItem1.DropDownItems[2].Text = "高级";
            startNewGame();
        }

        private void 高级ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            mineX = highX; mineY = highY;
            toolStripMenuItem1.DropDownItems[0].Text = "初级";
            toolStripMenuItem1.DropDownItems[1].Text = "中级";
            toolStripMenuItem1.DropDownItems[2].Text = "高级 *";
            startNewGame();
        }

        private void 退出ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void 外挂ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (plug_in)
            {
                plug_in = false;
                toolStripMenuItem2.DropDownItems[0].Text = "外挂";
                startNewGame();
            } 
            else
            {
                plug_in = true;
                toolStripMenuItem2.DropDownItems[0].Text = "外挂 *";
                startNewGame();
            }
        }

        private void 关于ToolStripMenuItem1_Click(object sender, EventArgs e)
        {

        }
    }
}
