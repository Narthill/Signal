#pragma once
//strtok�İ�ȫ�汾��windos��û�У�linux���У������Զ���
char *strtok_r(char *s, const char *delim, char **save_ptr) {  
	char *token;  

	if (s == NULL) s = *save_ptr;  

	/* Scan leading delimiters.  */  
	s += strspn(s, delim);  
	if (*s == '/0')   
		return NULL;  

	/* Find the end of the token.  */  
	token = s;  
	s = strpbrk(token, delim);  
	if (s == NULL)  
		/* This token finishes the string.  */  
		*save_ptr = strchr(token, '/0');  
	else {  
		/* Terminate the token and make *SAVE_PTR point past it.  */  
		*s = '/0';  
		*save_ptr = s + 1;  
	}  

	return token;  
}
//����ֵ0��1��2�ֱ��־�ַ�����double int
int  get_str_type(const char* pstr)
{
	bool bDot = false;

	if (pstr == NULL || *pstr == '\0')
	{
		return 0;
	}

	if ('-' == *pstr || '+' == *pstr)
	{//����ĸ����Ϊ ��������
		pstr++;
	}
	while(*pstr)
	{
		if ('.' == *pstr)
		{//ֻ�ܳ���һ��С����
			if (!bDot)
			{
				bDot = true;
				pstr++;
				continue;
			}
			return 0;
		}
		if ('0' > *pstr || '9' < *pstr)
		{
			return 0;
		}
		pstr++;
	}

	return bDot ? 1 : 2;
}