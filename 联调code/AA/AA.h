// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� AA_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// AA_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef AA_EXPORTS
#define AA_API __declspec(dllexport)
#else
#define AA_API __declspec(dllimport)
#endif

// �����Ǵ� AA.dll ������
class AA_API CAA {
public:
	CAA(void);
	// TODO: �ڴ�������ķ�����
};

extern AA_API int nAA;

AA_API int fnAA(void);
