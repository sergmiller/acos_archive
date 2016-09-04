#include "string_for_text_editor.h"
#include "cartesian_tree.h"
#include "filework.h"
#include "replace_function.h"
#include "my_print_pages.h"
#include "insert_functions.h"
#include "editor.h"

void tree_print(cartesian_tree_t* tree)
{
    if(tree == NULL)
    {
        printf("tree is null\n");
        return;
    }
    printf("tree->text: ");
    string_print(tree->text, stdout);
    printf("tree->size: %lu\n", tree->size);
    printf("tree->left: %p\n",(void*) tree->left);
    printf("tree->right: %p\n",(void*) tree->right);
    printf("tree->priority: %d\n", tree->priority);
    printf("\n");
}

/**base constructor for cartesian_tree**/
void cart_tree_construct(cartesian_tree_t** tree,string_t* str)
{
    cartesian_tree_t* new_tree = NULL;
    string_t* insert_str = NULL;

    if(!(new_tree = (cartesian_tree_t*)malloc(sizeof(cartesian_tree_t))))
    {
        system("clear");
        printf("malloc_error, can't allocate %lub\n", sizeof(cartesian_tree_t));
        exit(1);
    }
    if(!(insert_str = (string_t*)malloc(sizeof(string_t))))
    {
        system("clear");
        printf("malloc_error, can't allocate %lub\n", sizeof(string_t));
        exit(1);
    }

    insert_str->data = NULL;
    string_construct(&insert_str);
    string_big_push(insert_str, str->data, str->size);
    new_tree->text = insert_str;
    new_tree->size = 1;
    new_tree->left = NULL;
    new_tree->right = NULL;
    new_tree->priority = rand();
    *tree = new_tree;
}

/**basic merge of two lists of strings that relized in the form of cartesian_tree*/
void cart_tree_merge(cartesian_tree_t** merged_tree, cartesian_tree_t** left_tree, cartesian_tree_t** right_tree)
{
        cartesian_tree_t* res = NULL;
        cartesian_tree_t* buff_ptr = NULL;
        cartesian_tree_t* help_ptr = NULL;
    /**cases if one of merged tree is NULL**/
	if (*left_tree == NULL)
	{
        *merged_tree = *right_tree;
        recalc_params(*merged_tree);
        return;
    }
    /**/
	if (*right_tree == NULL)
	{
        *merged_tree = *left_tree;
        recalc_params(*merged_tree);
        return;
	}

	/**new root node is root node one of merged tree with max priority**/
	if ((*left_tree)->priority > (*right_tree)->priority)
	{
		*merged_tree = *left_tree;

		help_ptr = *left_tree;
		buff_ptr = help_ptr->right;
		cart_tree_merge(&res, &buff_ptr, right_tree);
		(*merged_tree)->right = res;
	}
	else
	{
		*merged_tree = *right_tree;

		help_ptr = *right_tree;
		buff_ptr = help_ptr->left;
		cart_tree_merge(&res, left_tree, &buff_ptr);
		(*merged_tree)->left = res;
	}

	recalc_params(*merged_tree);
}

/**basic split of list of strings in two lists: [0,index) and [index,size)
 *for correct work of split subtrees' sizes were used
 *also checking to null pointers was needed
 **/
void cart_tree_split(cartesian_tree_t** splited_tree_ptr, cartesian_tree_t** left_tree_ptr, cartesian_tree_t** right_tree_ptr, size_t index)
{
    cartesian_tree_t* splited_tree = NULL;
	cartesian_tree_t* left_tree = NULL;
	cartesian_tree_t* right_tree = NULL;
	int left_ind = index;

	if (*splited_tree_ptr == NULL)
		return;

	splited_tree = *splited_tree_ptr;
	left_tree = NULL;
	right_tree = NULL;

    /**sign of left_ind shows direction of new iteration**/
	if (splited_tree->left != NULL)
	{
		cartesian_tree_t* left_pointer;
		left_pointer = splited_tree->left;
		left_ind -= left_pointer->size;
	}


	if (left_ind < 0)
	{
		right_tree = splited_tree;
		cart_tree_split(&(right_tree->left), &left_tree, &(right_tree->left), index);
		recalc_params(left_tree);
		recalc_params(right_tree);
		*left_tree_ptr = left_tree;
		*right_tree_ptr = right_tree;
		return;
	}

	if (!left_ind)
	{
		left_tree = splited_tree->left;
		right_tree = splited_tree;
		right_tree->left = NULL;
		recalc_params(right_tree);
		recalc_params(left_tree);
		*left_tree_ptr = left_tree;
		*right_tree_ptr = right_tree;
		return;
	}

	if (left_ind > 0)
	{
		left_tree = splited_tree;
		cart_tree_split(&(left_tree->right), &(left_tree->right), &right_tree, left_ind - 1);
		recalc_params(right_tree);
		recalc_params(left_tree);
		*left_tree_ptr = left_tree;
		*right_tree_ptr = right_tree;
		return;
	}
}

/**insert new string in list in index position**/
void cart_tree_insert(cartesian_tree_t** tree, string_t* s, size_t index)
{
    cartesian_tree_t** left_tree = NULL;
    cartesian_tree_t** right_tree = NULL;
    cartesian_tree_t** insert_tree = NULL;
    cartesian_tree_t* left_pointer = NULL;
    cartesian_tree_t* right_pointer = NULL;
    cartesian_tree_t* insert_pointer = NULL;
    cartesian_tree_t* left_merge_ptr = NULL;
    /**case if tree was empty, new tree has one node**/
    if (*tree == NULL)
    {
        cart_tree_construct(tree, s);
        return;
    }
    /**init auxiliary buff trees**/
    left_tree = &left_pointer;
    right_tree = &right_pointer;

    cart_tree_split(tree, left_tree, right_tree, index);

    insert_tree = &insert_pointer;

    /**create new one-node tree**/
    cart_tree_construct(insert_tree, s);
    /**full merge**/
    cart_tree_merge(&left_merge_ptr, left_tree, insert_tree);
    cart_tree_merge(tree, &left_merge_ptr, right_tree);
}
/**insert new list in current list of strings in index position**/
void cart_tree_insert_tree(cartesian_tree_t** tree_ptr, cartesian_tree_t** inserted_tree_ptr, size_t index)
{
    cartesian_tree_t* left_tree = NULL;
    cartesian_tree_t* right_tree = NULL;
    cartesian_tree_t* sub_tree = NULL;
    cartesian_tree_t* inserted_tree = *inserted_tree_ptr;
    cartesian_tree_t* tree = *tree_ptr;

    cart_tree_split(&tree, &left_tree, &right_tree, index);
    cart_tree_merge(&sub_tree, &left_tree, &inserted_tree);
    cart_tree_merge(&tree, &sub_tree, &right_tree);

    *tree_ptr = tree;
}
/**access in string from index**/
void cart_tree_access(cartesian_tree_t** tree_ptr, size_t index, string_t** s)
{
    int left_ind = index;
    cartesian_tree_t* tree = NULL;

    if (*tree_ptr == NULL)
        return;

    tree = *tree_ptr;
    /**sign of left_ind shows direction of new iteration**/
    if (tree->left != NULL)
    {
        cartesian_tree_t* left_pointer;
        left_pointer = tree->left;
        left_ind -= left_pointer->size;
    }


    if (left_ind < 0)
    {
        cart_tree_access(&(tree->left), index, s);
        return;
    }

    if (!left_ind)
    {
        *s = tree->text;
        return;
    }

    if (left_ind > 0)
    {
        cart_tree_access(&(tree->right), (size_t)(left_ind - 1), s);
        return;
    }   

}

/**recalc correct params**/
void recalc_params(cartesian_tree_t* tree)
{
    cartesian_tree_t* check_left = NULL;

    cartesian_tree_t* check_right = NULL;
    if (tree == NULL)
        return;
    check_left = tree->left;
    check_right = tree->right;
    /**update subtrees size: it's new sum sizes of left and right trees + 1 **/
    tree->size = ((check_left == NULL)? 0 : check_left->size) + ((check_right == NULL) ? 0 : check_right->size) + 1;
}

/**print all strings from struct DFS IN-order**/
void print_tree(cartesian_tree_t* tree, FILE* fp)
{
    string_t* out_str = NULL;

	if (tree == NULL)
	{
        printf("List of strings is empty.\n");
		return;
	}
	if (tree->left != NULL)
        print_tree(tree->left, fp);
    
    out_str = tree->text;
    fprintf(fp, "%s\n", out_str->data);
    /**string_print(tree->text, fp);**/

	if (tree->right != NULL)
		print_tree(tree->right, fp);
}

void _real_print(cartesian_tree_t* tree, FILE* fp)
{
    string_t* out_str = NULL;

    if (tree == NULL)
    {
        printf("List of strings is empty.\n");
        return;
    }
    if (tree->left != NULL)
        _real_print(tree->left, fp);

    out_str = tree->text;
    fprintf(fp, "%s\n", out_str->data);

    if (tree->right != NULL)
        _real_print(tree->right, fp);
}

/**clear memory from tree**/
void tree_delete(cartesian_tree_t* tree)
{
    if (!tree)
        return;

    if(tree->left)
        tree_delete(tree->left);

    if(tree->right)
        tree_delete(tree->right);
    /**clear memory in current string**/
    string_delete(tree->text);
    free(tree->text);
    free(tree);
}

/*clear memory from double-linked list*/
void list_delete(cartesian_tree_t* list)
{
    if(!list)
        return;

    /*recursive finding last element in list*/
    if(list->right)
        list_delete(list->right);

    /**clear memory in current string**/
    string_delete(list->text);
}

void cart_tree_universal_action_with_subtree(cartesian_tree_t** tree_ptr, size_t left_ind, size_t right_ind, action_t action, data_warehouse_t* buff1, data_warehouse_t* buff2)
{
    cartesian_tree_t* left_tree = NULL;
    cartesian_tree_t* mid_tree = NULL;
    cartesian_tree_t* mid_left_tree = NULL;
    cartesian_tree_t* right_tree = NULL;
    cartesian_tree_t* tree = *tree_ptr;
  /*  printf("ins_ptr1: %d, ins_ptr23: %d\n", buff1, buff2);
    printf("ins_str1: %s, ins_str2: %s\n", buff1->data, buff2->data);*/
    cart_tree_split(&tree, &mid_left_tree, &right_tree, right_ind);
    cart_tree_split(&mid_left_tree, &left_tree, &mid_tree, left_ind - 1);
    
    if(action == DELETE_SUBTREE)
    {
        tree_delete(mid_tree);
        mid_left_tree = left_tree;
    }

    if(action == DELETE_BRACES)
    {
        in_order_delete_braces(&mid_tree);
        cart_tree_merge(&mid_left_tree, &left_tree, &mid_tree);
    }

    if(action == REPLACE)
    {
        in_order_replace(&mid_tree, buff1, buff2);
        cart_tree_merge(&mid_left_tree, &left_tree, &mid_tree);
    }    

    if(action == PRINT)
    {
         /**clear window**/
        system("clear");
        print_pages(mid_tree, left_ind);
        cart_tree_merge(&mid_left_tree, &left_tree, &mid_tree);
    }

    cart_tree_merge(&tree, &mid_left_tree, &right_tree);
    *tree_ptr = tree;
}
/********************************************there is build replace-func*************************************************************/
void in_order_replace(cartesian_tree_t** mid_tree, data_warehouse_t* buff1, data_warehouse_t* buff2)
{
    int* pref_func = NULL;
    short cnt_indissoloble = 1;
    size_t range = 0;
    char* next_common_substr = NULL;

    string_t* pref_str = NULL;
    string_t* scan_str = NULL;
    string_t* buff_str = NULL;
    string_t* begin_ptr = NULL;
    string_t* end_ptr = NULL;

    cartesian_tree_t* insert_list_begin = NULL;
    cartesian_tree_t* insert_list_end = NULL;
    cartesian_tree_t* list = NULL;
    cartesian_tree_t* end_list = NULL;
    cartesian_tree_t* ptr = NULL;
    cartesian_tree_t* first_cur_ptr = NULL;
    cartesian_tree_t* tree = *mid_tree;
    cartesian_tree_t* buff_tree = NULL;
    cartesian_tree_t* left_ptr = NULL;
    cartesian_tree_t* next_ptr = NULL;

    if(buff1 && buff2)
    {
        pref_str = buff1->str_ptr;
        get_pref_function(pref_str, &pref_func);
    }
    
    if(buff1 && buff1->list_begin)
    {
        insert_list_begin = buff1->list_begin;
        insert_list_end = buff1->list_end;
        cnt_indissoloble = 0;
    }
    else if(buff2 && buff2->list_begin)
    {
        insert_list_begin = buff2->list_begin;
        insert_list_end = buff2->list_end;
        cnt_indissoloble = 0;
    }
   
    if(cnt_indissoloble)
        dfs_replace(mid_tree, (buff1 ? buff1->str_ptr : NULL), (buff2 ? buff2->str_ptr : NULL), &pref_func);
    
    else
    {
        conv_tree_to_list(&list, &end_list, &tree);
        ptr = list; 

        while(1)
        {
            scan_str = ptr->text;
            range = 0;

            while(1)
            {

                /*division of strings*/
                next_common_substr = get_next_substr(scan_str->data + range, pref_str->data, pref_str->size, &pref_func);
                if(next_common_substr)/*<--it's '\n'*/
                {
                    buff_str = NULL;
                    buff_tree = NULL;
                    string_construct(&buff_str);
                    string_big_push(buff_str, scan_str->data + range, (size_t)(next_common_substr - (scan_str->data + range)));
                    cart_tree_construct(&buff_tree, buff_str);
                    range = (size_t)(next_common_substr + pref_str->size - scan_str->data);

                    if(ptr == list)
                    {
                        list = buff_tree;
                        first_cur_ptr = list;
                    }
                    else
                    {
                        left_ptr = ptr->left;
                        left_ptr->right = buff_tree;
                        buff_tree->left = left_ptr;
                    }
                    
                    buff_tree->right = ptr;
                    ptr->left = buff_tree;
                }
                else
                {
                    /*end of line*/
                    string_big_remove(scan_str, scan_str->data, range);
                    break;
                }
            }
            /*add strings between previous division strings*/
            /*********************************************************************************/
            if(first_cur_ptr != list)
                first_cur_ptr = first_cur_ptr->right;
            
            while(first_cur_ptr != ptr)
            {
                next_ptr = first_cur_ptr->right;
                begin_ptr = insert_list_begin->text;
                end_ptr = insert_list_end->text;

                string_big_push(first_cur_ptr->text, begin_ptr->data, begin_ptr->size);
                string_big_insert(next_ptr->text, 0, end_ptr->data, end_ptr->size);
                insert_copy_list(first_cur_ptr, insert_list_begin, insert_list_end);
                first_cur_ptr = next_ptr;
            }
            /**********************************************************************************/
            first_cur_ptr = ptr;

            if(ptr != end_list)
                ptr = ptr->right;
            else
                break;
        }

        conv_list_to_tree(&list, &end_list, &tree);   
        *mid_tree = tree; 
    }

    free(pref_func);
}

void dfs_replace(cartesian_tree_t** tree_ptr, string_t* buff1, string_t* buff2, int** pref)
{
    cartesian_tree_t* tree = *tree_ptr;
    string_t* replaced_str = NULL;
    string_t* inserted_str = NULL;
    string_t* scan_str = tree->text;
    short replace_param = 0;
    size_t shift = 0;

    if(buff1 == NULL)
    {
        replace_param = 2;
        inserted_str = buff2;
        shift = scan_str->size;
    }
    else
    {
        if(buff2 == NULL)
        {
            replace_param = 1;
            inserted_str = buff1;
        }
        else
        {
            replaced_str = buff1;
            inserted_str = buff2;
        }
    }

    if(tree->left != NULL)
    dfs_replace(&(tree->left), buff1, buff2, pref);

    if(!replace_param)
        replace_substr(inserted_str, replaced_str, &scan_str, pref);
    else/*case if insert in begin or end of all strings*/
    {
        string_big_insert(scan_str, shift, inserted_str->data, inserted_str->size);
       /* string_replace(scan_str, shift, inserted_str, 0);*/
    }

    tree->text = scan_str;
    tree_ptr = &tree;

    if(tree->right != NULL)
        dfs_replace(&(tree->right), buff1, buff2, pref);
}
/********************************************there is build replace-func*************************************************************/
void in_order_delete_braces(cartesian_tree_t** tree_ptr)
{
    int deep_level = 0;
    short err = 0;
    string_t* cur_str = NULL;
    cartesian_tree_t* tree = *tree_ptr;
    cartesian_tree_t* left_data = NULL;
    cartesian_tree_t* right_data = NULL;
    cartesian_tree_t* list = NULL;
    cartesian_tree_t* end_list = NULL;
    cartesian_tree_t* ptr = NULL;

    conv_tree_to_list(&list, &end_list, &tree);

    ptr = list;

    while(1)
    {
        err = string_scan_braces(&deep_level, ptr->text);
        if(err < 0)
        {
            fprintf(stderr, "Can't do this operation: braces aren't balanced.\n");
            break;
        }

        if(ptr == end_list)
        {
            if(deep_level != 0)
            {   
                err = -1;
                fprintf(stderr, "Can't do this operation: braces aren't balanced.\n");
            }
            break;
        }

        ptr = ptr->right;
    }

    if(!err)
    {
        ptr = list;

        while(1)
        {
            string_delete_braces(&deep_level,ptr->text);

            cur_str = ptr->text;
            if(*cur_str->data == '\0')
            {
                left_data = ptr->left;
                right_data = ptr->right;

                ptr->left = NULL;
                ptr->right = NULL;
                tree_delete(ptr);

                if(left_data)
                    left_data->right = right_data;
                else
                    list = right_data;

                if(right_data)
                    right_data->left = left_data;
                else
                {
                    end_list = left_data;
                    break;
                }

                ptr = right_data;
            }
            else
            {
                if(ptr == end_list)
                    break;

                ptr = ptr->right;
            }
        }
    }

    conv_list_to_tree(&list, &end_list, &tree);   
    *tree_ptr = tree; 
}

/*convert cartesian_tree in double-linked list, getting begin and end of list*/
void conv_tree_to_list(cartesian_tree_t** list_ptr, cartesian_tree_t** end_list_ptr, cartesian_tree_t** tree_ptr)
{
    cartesian_tree_t* list = *list_ptr;
    cartesian_tree_t* end_list = *end_list_ptr;
    cartesian_tree_t* tree = *tree_ptr;
    cartesian_tree_t* right_list = NULL;
    cartesian_tree_t* left_end_list = NULL;

    if(tree->left == NULL)
        list = tree;
    else
    {
        conv_tree_to_list(&list, &left_end_list, &(tree->left));
        left_end_list->right = tree;
        tree->left = left_end_list;
    }

    if(tree->right == NULL)
        end_list = tree;
    else
    {
        conv_tree_to_list(&right_list, &end_list, &(tree->right));
        right_list->left = tree;
        tree->right = right_list;
    }

    *list_ptr = list;
    *end_list_ptr = end_list;
    tree = NULL;
    *tree_ptr = tree;
}

void conv_list_to_tree(cartesian_tree_t** list_ptr, cartesian_tree_t** end_list_ptr, cartesian_tree_t** tree_ptr)
{
    cartesian_tree_t* list = *list_ptr;
    cartesian_tree_t* end_list = *end_list_ptr;
    cartesian_tree_t* tree = *tree_ptr;
    cartesian_tree_t* ins_tree = NULL;
    cartesian_tree_t* buff_tree = NULL;

    while(list != NULL)
    {
        ins_tree = list;

        if(list != end_list)
            list = list->right;
        else
            list = NULL;

        ins_tree->right = NULL;
        ins_tree->left = NULL;
        cart_tree_merge(&buff_tree, &tree, &ins_tree);
        tree = buff_tree;
    }
    *tree_ptr = tree;
}

void insert_copy_list(cartesian_tree_t* cur_ptr, cartesian_tree_t* list_begin, cartesian_tree_t* list_end)
{
    cartesian_tree_t* next_ptr = cur_ptr->right;
    cartesian_tree_t* buff_tree = NULL;
    string_t* buff_str = NULL;

    list_begin = list_begin->right;

    while(list_begin != list_end)
    {
        buff_str = NULL;
        string_construct(&buff_str);
        *buff_str = *(list_begin->text);

        buff_tree = NULL;
        cart_tree_construct(&buff_tree, buff_str);
        buff_tree->left = cur_ptr;
        cur_ptr->right = buff_tree;
        cur_ptr = buff_tree;
        list_begin = list_begin->right;
    }
    cur_ptr->right = next_ptr;
    next_ptr->left = cur_ptr;
}
