import os
import asyncio
from openai import AsyncOpenAI
 
client = AsyncOpenAI(
  api_key=os.environ.get('OPENAI_API_KEY'),
)

async def generate():
    content_rfc = open('ICS_docs/ftprfc.txt').read()
    content_code = open('ICS_docs/code.c').read()
    content_iec = open('ICS_docs/TR-IEC104.txt').read()
    question = 'Refer to the ftp RFC document and implementation code, output the "region_t* extract_requests_IEC104(unsigned char* buf, unsigned int \
    buf_size, unsigned int* region_count_ref)" and "unsigned int* extract_response_codes_IEC104(unsigned char* buf, unsigned int buf_size, unsigned int* state_count_ref)" according to the IEC protocol document'
    completion = await client.chat.completions.create(
      model="gpt-4o",
      messages=[
        {"role": "user", "content": content_rfc + '\n\n' + content_code + '\n\n' + content_iec + '\n\n' + question}
      ],
      stream=True,
    )
    async for chunk in completion:
        print(chunk.choices[0].delta.content or "", end="", flush=True)
    print()
 
asyncio.run(generate())