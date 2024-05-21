//
// Created by 孟佳颖 on 2024/5/21.
//

#include "stdio.h"
#include "stdlib.h"

#pragma pack (push,1)
typedef struct mbap_be{
    unsigned short tid; //transaction id: 2 Bytes
    unsigned short protocol; //protocol identifier: 2 Bytes
    unsigned short length; //length: 2 Bytes = uid + data fields
    unsigned char uid; //unit id: 1 Bytes
    unsigned char fid; //function code: 1 Bytes
}mbap_be;
#pragma pack(pop)

typedef struct {
    int start_byte; //开始的比特
    int end_byte;   //结束的比特
    char modifiable;  //可修改的flag信息
    unsigned int *state_sequence; //保存的从服务器反馈的状态信息
    unsigned int state_count; //state_sequence中保存的状态数量
}region_t;

#define ushort_be_to_se(v) ((v & 0xff) << 8) + ((v & 0xff00) >> 8)

region_t* extract_requests_modbus(unsigned char* buf, unsigned int buf_size, unsigned int* region_count_ref)
{
    unsigned char* end_ptr = buf + buf_size -1;
    unsigned char* cur_ptr = buf;
    unsigned int region_count = 0;
    unsigned int cur_start = 0;
    region_t* regions = NULL;

    if (buf == NULL || buf_size == 0){
        *region_count_ref = region_count;
        return regions;
    }

    while (cur_ptr <= end_ptr){
        unsigned int remaining_buf_size = end_ptr - cur_ptr + 1;
        //MBAP + function code = 8 Bytes
        if (remaining_buf_size >= sizeof(mbap_be)){
            region_count ++;
            regions = (region_t *)ck_realloc(regions, region_count*sizeof(region_t)); //Re-allocate a buffer, checking for issues and zeroing any newly-added tail

            regions[region_count-1].state_sequence = NULL;
            regions[region_count-1].state_count = 0;
            regions[region_count-1].start_byte = cur_start;

            // check data region
            mbap_be *header = (mbap_be *)cur_ptr;
            // data field <= 252 bytes for valid packet
            // length field = uid + fid + data <= 254
            unsigned short remaining_packet_length = ushort_be_to_se(header->length);
            remaining_packet_length = (remaining_packet_length > 254) ? 254 : remaining_packet_length;
            unsigned short data_length = remaining_packet_length - 2;
            unsigned int packet_length = remaining_packet_length + 6;
            unsigned short available_data_length = (remaining_buf_size >= packet_length) ? data_length : remaining_buf_size - sizeof(mbap_be);

            if (available_data_length > 0){
                cur_ptr = cur_ptr + sizeof(mbap_be) + available_data_length -1;
                regions[region_count -1].end_byte = cur_ptr;
            }else {
                break;
            }

        }else {
            //malformed
            if (remaining_buf_size > 0){
                region_count = region_count +1;
                regions = (region_t *)ck_realloc(regions, region_count*sizeof(region_t));
                regions[region_count-1].start_byte = cur_start;
                regions[region_count-1].end_byte = cur_start + remaining_buf_size -1;
                regions[region_count-1].state_sequence = NULL;
                regions[region_count-1].state_count = 0;
            }
            break;
        }
    }

    *region_count_ref = region_count; //region_count表示buf中一共有多少个field
    return regions; //将buf中每个field的开始结束位置标记
}

int main(){
    unsigned char a[11] = {0x00, 0x01, 0x00, 0x00, 0x06, 0x00, 0xff, 0x05, 0x00, 0x00, 0xff};
    unsigned char* buf = a;
    mbap_be* header = (mbap_be *)buf;
    unsigned short b = header->length;
    printf("%d",b);
}
