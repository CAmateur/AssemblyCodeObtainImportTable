//ע�⣬�ر���Ŀ�е��漴��ַ
//�����̶���ַ

#include<stdio.h>
#include<Windows.h>
//_delcspec(naked)����������д��C������Ƕ������һЩ�ض����ܡ�
//__declspec(naked)����Ӧ�ò��д
//��mov��ֵ��ʱ��ռ�ñ�Ҫ�ļĴ�����������ǰpushһ�¼Ĵ���
//cmp eax,ebx
//je XXX
//��eax��ebx����������Ϊ��֤����ȣ�je�������Ⱦ���ת
//je=jz

void __declspec(naked) AssemblyCode()
{

	__asm
	{
		push ebp

		mov ebp, esp
		sub esp, 0x40


		push eax
		push ecx
		push edi
		push ebx
		push edx


		//repָ���Ŀ�����ظ��������ָ��.ECX��ֵ���ظ��Ĵ���.
		//STOSָ��������ǽ�eax�е�ֵ������ES : EDIָ��ĵ�ַ.
		mov eax, 0xCCCCCCCC
		mov ecx, 0x10        //Ҳ����˵���Դ�16���ֲ�����
		lea edi, dword ptr es : [ebp - 0x40]
		rep stosd
		///////////////////////////////////////////////////////////////////////
		//����Լ��Ĵ���
		//���õ����ݼĴ�����eax/ebx/ecx/edx

		//������ĵ�ַ���ھֲ�����һ(AddressBase)��λ��
		mov dword ptr ss : [ebp - 0x4], 0x00400000
		mov ebx, dword ptr ss : [ebp - 0x4]

		//��e_lfanew���ھֲ�������(AddressE_lfanew)��
		add ebx, 0x3c
		mov dword ptr ss : [ebp - 0x8], ebx

		mov eax, dword ptr ds : [ebx]
		add eax, dword ptr ss : [ebp - 0x4]

		//��Image_Nt_Headers�ĵ�ַ���ھֲ���������(AddressNtHeaders)��
		mov dword ptr ss : [ebp - 0xC], eax

		//��Image_Data_Directory����ĵ�ַ���ھֲ�������(AddresssDataDirectory)��
		add eax, 0x78
		mov dword ptr ss : [ebp - 0x10], eax

		
		mov ebx, dword ptr ds : [eax + 0x8]

		add ebx, dword ptr ds : [ebp - 0x4]
		//��Import���ڴ��ַ����ֲ�������(AddressImportTable)��
		mov dword ptr ss : [ebp - 0x14], ebx

		//���Dll���Ƶ�ַ
		mov edx, dword ptr ds : [ebx + 0xC]
		add edx, dword ptr ss : [ebp - 0x4]


		//����KERNEL32.dll�ַ���
		mov byte ptr ss : [ebp - 0x24], 0x4B
		mov byte ptr ss : [ebp - 0x23], 0x45
		mov byte ptr ss : [ebp - 0x22], 0x52
		mov byte ptr ss : [ebp - 0x21], 0x4E
		mov byte ptr ss : [ebp - 0x20], 0x45
		mov byte ptr ss : [ebp - 0x1F], 0x4C
		mov byte ptr ss : [ebp - 0x1E], 0x33
		mov byte ptr ss : [ebp - 0x1D], 0x32
		mov byte ptr ss : [ebp - 0x1C], 0x2E
		mov byte ptr ss : [ebp - 0x1B], 0x64
		mov byte ptr ss : [ebp - 0x1A], 0x6C
		mov byte ptr ss : [ebp - 0x19], 0x6C
		mov byte ptr ss : [ebp - 0x18], 0x00

		//���㿪ʼѭ������13�μ��ַ���KERNEL32.dll�ĳ���
		mov ecx, 0
		mov edi, 0x1
		J2 : mov eax, 0x24
			 
			 mov bl, byte ptr ds : [edx + ecx]
			 sub eax, ecx
			 add ecx, 0x1
			 push ebp
			 sub ebp, eax

			 //���һ���ַ���ȣ�����ת��J1���ж�13���Ƿ����
			 cmp byte ptr ss : [ebp], bl
			 pop ebp
			 je J1

			 //����ж�һ���ַ�����Ⱦͽ�ָ��ָ����һ���ṹ��
			 mov ecx, 0x0		//����������
			 mov edx, dword ptr ss : [ebp - 0x14]
			 push eax
			 
			 //mul����һ��32λ�Ĵ��뽫eax��ֵ��mul������ļĴ�����ֵ��ˣ��������λ����edx��������ĵ�λ����eax
			 push edx
			 mov eax, 0x14
			 mul edi
			 pop edx


			//�õ��ṹ��IMPROT_DESCRIPTOR��ָ��
			 add edx, eax
			 //��ָ�����ֲ�������
			 mov dword ptr ss : [ebp - 0x28], edx
			 add edi, 0x1
			//�õ�ָ�룬ָ��Name��ָ��
			 add edx, 0xC
			 pop eax


			//��ȡָ��Name��ָ��õ�RVA
			 mov edx, dword ptr ds : [edx]
			//�õ��ܶ�ȡ��Ŀ���ַ
			 add edx, dword ptr ss : [ebp - 0x4]
			 jmp J2		//����ָ��ɹ�����J2���½����ж�


			//�жϳ����Ƿ�ﵽ
			 J1 : cmp ecx, 0xC
				  jne J2	//���δ�ﵽ����J2�����ж�
			//�ҵ���Ҫ��KERNEL32.dll��ָ��IMPROT_DESCRIPTOR�ṹ��ָ�룬��ָ����Ϊ����ֵ����eax
				  mov eax, dword ptr ss : [ebp - 0x28]




				  ///////////////////////////////////////////////////////////////////////
				  pop edx
				  pop ebx
				  pop edi
				  pop ecx

				  add esp, 4
				  mov esp, ebp
				  pop ebp
				  //ret �൱��add esp,0x4
				  //ret 4 �൱��add esp,0x8
				  ret



	}


}


int main()
{
	


	AssemblyCode();//����KERNEL32.dllImportTableAddress
	DWORD ImportKernel32Address = NULL;
	//ȡ��������ֵ
	__asm
	{
		mov ImportKernel32Address, eax
	}

	printf("ImportKernel32Address:%X\n", ImportKernel32Address);
	getchar();
	return 0;
}