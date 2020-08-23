//注意，关闭项目中的随即地址
//开启固定地址

#include<stdio.h>
#include<Windows.h>
//_delcspec(naked)用在驱动编写，C语言内嵌汇编完成一些特定功能。
//__declspec(naked)用在应用层编写
//用mov传值的时候，占用必要的寄存器，可以提前push一下寄存器
//cmp eax,ebx
//je XXX
//将eax与ebx相减，结果若为零证明相等，je：如果相等就跳转
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


		//rep指令的目的是重复其上面的指令.ECX的值是重复的次数.
		//STOS指令的作用是将eax中的值拷贝到ES : EDI指向的地址.
		mov eax, 0xCCCCCCCC
		mov ecx, 0x10        //也就是说可以存16个局部变量
		lea edi, dword ptr es : [ebp - 0x40]
		rep stosd
		///////////////////////////////////////////////////////////////////////
		//添加自己的代码
		//可用的数据寄存器：eax/ebx/ecx/edx

		//将程序的地址存在局部变量一(AddressBase)的位置
		mov dword ptr ss : [ebp - 0x4], 0x00400000
		mov ebx, dword ptr ss : [ebp - 0x4]

		//将e_lfanew存在局部变量二(AddressE_lfanew)中
		add ebx, 0x3c
		mov dword ptr ss : [ebp - 0x8], ebx

		mov eax, dword ptr ds : [ebx]
		add eax, dword ptr ss : [ebp - 0x4]

		//将Image_Nt_Headers的地址存在局部变量三种(AddressNtHeaders)中
		mov dword ptr ss : [ebp - 0xC], eax

		//将Image_Data_Directory数组的地址存在局部变量四(AddresssDataDirectory)中
		add eax, 0x78
		mov dword ptr ss : [ebp - 0x10], eax

		
		mov ebx, dword ptr ds : [eax + 0x8]

		add ebx, dword ptr ds : [ebp - 0x4]
		//将Import的内存地址存入局部变量五(AddressImportTable)中
		mov dword ptr ss : [ebp - 0x14], ebx

		//获得Dll名称地址
		mov edx, dword ptr ds : [ebx + 0xC]
		add edx, dword ptr ss : [ebp - 0x4]


		//存入KERNEL32.dll字符串
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

		//从零开始循环次数13次即字符串KERNEL32.dll的长度
		mov ecx, 0
		mov edi, 0x1
		J2 : mov eax, 0x24
			 
			 mov bl, byte ptr ds : [edx + ecx]
			 sub eax, ecx
			 add ecx, 0x1
			 push ebp
			 sub ebp, eax

			 //如果一个字符相等，就跳转到J1再判断13次是否完成
			 cmp byte ptr ss : [ebp], bl
			 pop ebp
			 je J1

			 //如果判断一个字符不相等就将指针指向下一个结构体
			 mov ecx, 0x0		//将计数清零
			 mov edx, dword ptr ss : [ebp - 0x14]
			 push eax
			 
			 //mul操作一个32位寄存齐将eax的值与mul后面跟的寄存器的值相乘，将结果高位存入edx，将结果的低位存入eax
			 push edx
			 mov eax, 0x14
			 mul edi
			 pop edx


			//得到结构体IMPROT_DESCRIPTOR的指针
			 add edx, eax
			 //将指针存入局部变量中
			 mov dword ptr ss : [ebp - 0x28], edx
			 add edi, 0x1
			//得到指针，指向Name的指针
			 add edx, 0xC
			 pop eax


			//读取指向Name的指针得到RVA
			 mov edx, dword ptr ds : [edx]
			//得到能读取的目标地址
			 add edx, dword ptr ss : [ebp - 0x4]
			 jmp J2		//更换指针成功跳回J2重新进行判断


			//判断长度是否达到
			 J1 : cmp ecx, 0xC
				  jne J2	//如果未达到跳回J2继续判断
			//找到想要的KERNEL32.dll，指向IMPROT_DESCRIPTOR结构的指针，将指针作为返回值传给eax
				  mov eax, dword ptr ss : [ebp - 0x28]




				  ///////////////////////////////////////////////////////////////////////
				  pop edx
				  pop ebx
				  pop edi
				  pop ecx

				  add esp, 4
				  mov esp, ebp
				  pop ebp
				  //ret 相当于add esp,0x4
				  //ret 4 相当于add esp,0x8
				  ret



	}


}


int main()
{
	


	AssemblyCode();//查找KERNEL32.dllImportTableAddress
	DWORD ImportKernel32Address = NULL;
	//取函数返回值
	__asm
	{
		mov ImportKernel32Address, eax
	}

	printf("ImportKernel32Address:%X\n", ImportKernel32Address);
	getchar();
	return 0;
}