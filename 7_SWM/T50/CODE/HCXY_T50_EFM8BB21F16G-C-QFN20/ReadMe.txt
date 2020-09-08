X30一定要确认，拉地的时候不能报警，余震过大不能报警
20190514
1. 更改OBSTACLE_START_VOL   3-->4
20190612
    1. 修复上电后，MP5一直有状态条的bug,方法为修改无障碍物时，默认数据为0x19。
201906131530
	1. 修改感度表，增大100cm之后的感度表，使其在100cm之后仅大墙报警；
	2. 修改回波分析快结束的位置为：
	          //voltage signel height donot increase to time exit check; use to sampling voltage is relatively low
            if( ObsStartThre < (DETECT_NOISE_BASE_VOL << 2) &&
                AnsData.bObsSigVolHeight < (ObsStartThre << 1) &&
                AnsData.wObsSigTimeWidth < ((WORD)COUNT_TIME_UNIT<<4)) // sampling of 8 time
			{
				if(++AnsData.wObsSigExitCnt >= 16)
				{
					AnsData.bObsSigStartFlag = FALSE;
				}
			}
	3. 上电后延时增加300ms,目前为500ms      201906131730
	4.修改上电余震小于1000时，赋值为1000，解决上电有时会长鸣的问题  --201906131830
	
  20190625  
    1. 修改Cpu_Clk_Init()函数，解决高温不工作的问题；

20190826
    1. 修改余震最大值，以适应库房呆滞探芯