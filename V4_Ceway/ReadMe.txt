�������޸ĵĵط���
1.Main�����У�
  	double deltaZ_p = p_num - ori_num;
  	double deltaZ_m = m_num - ori_num;
  ��Ҫ�����޸ģ�
  	double adjust_z = state.deltaZ;
	double adjust_u = state.x_angle * -1;
	double adjust_v = state.y_angle * -1;
  ά�ֲ���
2.state���������˲���scale

3.�ܶ��������ֱ���趨����state�������Ҫ������Ų����������

4.������ѡ���жϹ��ܣ������жϹ��ܣ�����Ϊcout�������������г���


��ע�⣬�˰汾��Adjust��������v1�����෴