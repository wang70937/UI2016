#include "stdafx.h"

// ���ڽ��һ�����У��������͵�item֮�� id �����ظ�������

#define TYPE_ENCODE_OFFSET  60   // ����λ����item type
#define KEY_TYPE_MASK    ((1<<4)-1)
#define KEY_REALID_MASK  (((LONGLONG)1<<TYPE_ENCODE_OFFSET)-1)

// ����
LONGLONG  encode_id(long type, LONGLONG lId);
LONGLONG  decode_id(LONGLONG lId, long* ptye);

LONGLONG  encode_id(long type, LONGLONG lId)
{
    LONGLONG lType = (LONGLONG)type;
    LONGLONG lEncodeId = (lType << TYPE_ENCODE_OFFSET) | lId;
    return lEncodeId;
}
LONGLONG  decode_id(LONGLONG lId, long* ptype)
{
    LONGLONG lMask = KEY_TYPE_MASK;
    if (ptype)
        *ptype = (lId >> TYPE_ENCODE_OFFSET) & lMask;

    lMask = KEY_REALID_MASK;
    LONGLONG lRealId = lId & lMask;
    return lRealId;
}