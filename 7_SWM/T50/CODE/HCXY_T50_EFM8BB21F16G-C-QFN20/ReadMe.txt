X30һ��Ҫȷ�ϣ����ص�ʱ���ܱ�������������ܱ���
20190514
1. ����OBSTACLE_START_VOL   3-->4
20190612
    1. �޸��ϵ��MP5һֱ��״̬����bug,����Ϊ�޸����ϰ���ʱ��Ĭ������Ϊ0x19��
201906131530
	1. �޸ĸжȱ�����100cm֮��ĸжȱ�ʹ����100cm֮�����ǽ������
	2. �޸Ļز������������λ��Ϊ��
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
	3. �ϵ����ʱ����300ms,ĿǰΪ500ms      201906131730
	4.�޸��ϵ�����С��1000ʱ����ֵΪ1000������ϵ���ʱ�᳤��������  --201906131830
	
  20190625  
    1. �޸�Cpu_Clk_Init()������������²����������⣻

20190826
    1. �޸��������ֵ������Ӧ�ⷿ����̽о