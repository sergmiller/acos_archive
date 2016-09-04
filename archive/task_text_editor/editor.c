#include "editor.h"
#include "string_for_text_editor.h"

void get_pref_function(string_t* str_ptr, int** pref)
{
	size_t i = 1;
	size_t j = 0;
	int* pref_func = NULL;
	char* str = str_ptr->data;
	size_t size = str_ptr->size;
	
	*pref = (int*)malloc((size + 1) * sizeof(int));

	pref_func = *pref;

	pref_func[0] = 0;
	for(;i <= size; ++i)
	{
		j = pref_func[i-1];
		while(j > 0 && str[i] != str[j])
			j = pref_func[j-1];

		if(str[i] == str[j])
			++j;

		pref_func[i] = j;
	}
/*
	pref_func[size + 1] = -1;
	*/
}

/*KMP - algorithm for find first substring is pref_str in scan_str*/
void replace_substr(string_t* inserted_str, string_t* pref_str_ptr, string_t** scan_str_ptr, int** pref_func)
{
	string_t* scan_str = *scan_str_ptr;
	int i = 0;
	/*size_t pref_val =  0;*/
	char* pref_str = pref_str_ptr->data;
	char* common_substr = NULL;
	size_t substr_size = pref_str_ptr->size;

	
	common_substr = get_next_substr(scan_str->data, pref_str, substr_size, pref_func);

	while(common_substr)
	{ 
		i = common_substr - scan_str->data;
		i += inserted_str->size;
		/*
		string_replace(scan_str_ptr, (size_t)(common_substr - scan_str), inserted_str, substr_size);
		*/
		string_big_remove(scan_str, common_substr, substr_size);
		string_big_insert(scan_str, (size_t)(common_substr - scan_str->data), inserted_str->data, inserted_str->size);
		common_substr = get_next_substr(scan_str->data + i, pref_str, substr_size, pref_func);
	}

	scan_str_ptr = &scan_str;
}

char* get_next_substr(char* scan_str, char* pref_str, size_t finding_str_size, int** pref_func)
{
	int* pref = *pref_func;
	int i = 0;
	int j = 0;
	size_t pref_val =  0;

	while(scan_str[i] != '\0')
	{
		j = pref_val;

		while(j > 0 && scan_str[i] != pref_str[j])
			j = pref[j - 1];

		if(scan_str[i] == pref_str[j])
			++j;

		pref_val = j;

		if(pref_val == finding_str_size)
			return scan_str + (i - finding_str_size + 1);

		++i;
	}

	return NULL;
}






