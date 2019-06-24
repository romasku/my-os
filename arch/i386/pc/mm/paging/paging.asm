global invalidate_page:function

invalidate_page:
    mov 	eax, [esp + 4]
    invlpg [eax]
    ret