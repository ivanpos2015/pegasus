#ifndef __DEFINE_PEGASUS_EXTERNAL_INTERFACE__
#define __DEFINE_PEGASUS_EXTERNAL_INTERFACE__
///
//
///
#include "register_idx.h"
///
// interface
///
#define get_bit_flag(t, i)		(t >> i) & 1
#define GetFlagBit(eflags, i)	get_bit_flag(eflags, i)

#define CF_INDEX	0
#define PF_INDEX	2
#define AF_INDEX	4
#define ZF_INDEX	6
#define SF_INDEX	7
#define TF_INDEX	8
#define IF_INDEX	9
#define DF_INDEX	10
#define OF_INDEX	11
#define IOPL_INDEX_1	12
#define IOPL_INDEX_2	13
#define NT_INDEX		14
#define RF_INDEX		16
#define VM_INDEX		17
#define AC_INDEX		18
#define VIF_INDEX		19
#define VIP_INDEX		20
#define ID_INDEX		21

#pragma pack(push, 1)
typedef struct _SegmentDescriptor {
	union {
		struct {
			unsigned short limit_low;
			unsigned short base_low;
			unsigned char base_mid;
			unsigned char type : 4;
			unsigned char system : 1;
			unsigned char dpl : 2;
			unsigned char present : 1;
			unsigned char limit_hi : 4;
			unsigned char available : 1;
			unsigned char is_64_code : 1;
			unsigned char db : 1;
			unsigned char granularity : 1;
			unsigned char base_hi;
		};
		unsigned long long descriptor; // resize 8byte.
	};
}SegmentDescriptor, *PSegmentDescriptor;
#pragma pack(pop)

namespace engine
{
	class debugger
	{
	public:
		virtual ~debugger() {}

		virtual bool __stdcall is_64_cpu() = 0;

		virtual void __stdcall print_code(unsigned long long ip, unsigned long line) = 0;
		virtual void * __stdcall get_windbg_linker() = 0;

		virtual bool __stdcall mnemonic_mov_gs(void *engine, unsigned long long ip) = 0;
		virtual bool __stdcall mnemonic_mov_ss(void *engine, unsigned long long ip) = 0;

		virtual void __stdcall log_print() = 0;
		virtual size_t __stdcall alignment(size_t region_size, unsigned long image_aligin) = 0;
		virtual void __stdcall current_regs() = 0;
		virtual cpu_context_type __stdcall get_current_thread_context() = 0;

		virtual bool __stdcall attach(void *mem) = 0;
		virtual bool __stdcall switch_cpu(void *mem) = 0;
		virtual bool __stdcall reboot(void *mem) = 0;
		virtual bool __stdcall trace_ex(void *mem) = 0;

		virtual bool __stdcall load(void *address) = 0;

		virtual bool __stdcall set_environment_block() = 0;
		virtual bool __stdcall create_global_descriptor_table_ex() = 0;

		virtual bool __stdcall setting(char *path) = 0;
		virtual bool __stdcall store() = 0;
		virtual bool __stdcall query_storage_memory(unsigned long long value, wchar_t *file_name, size_t *size) = 0;
		virtual unsigned char * __stdcall load_storage_memory(unsigned long long value, unsigned long long *base, size_t *size) = 0;
		virtual bool __stdcall load_page(unsigned long long value) = 0;
		virtual bool __stdcall load_context(void *mem) = 0;

		virtual bool __stdcall query(unsigned long long address, unsigned long long *base, size_t *size) = 0;
		virtual bool __stdcall read(unsigned long long address, unsigned char *dump, size_t *size) = 0;
	};

	class linker
	{
	public:
		virtual ~linker() {}
		virtual void __stdcall setting(const char *argument_str, int *argument_count, char(*args)[1024]) = 0;

		virtual unsigned long long __stdcall get_teb_address() = 0;
		virtual unsigned long long __stdcall get_peb_address() = 0;

		virtual bool __stdcall virtual_query(unsigned long long address, void *context, size_t context_size) = 0;
		virtual bool __stdcall virtual_query(unsigned long long address, MEMORY_BASIC_INFORMATION64 *mbi) = 0;
		virtual unsigned long __stdcall read_memory(unsigned long long address, void *buffer, size_t buffer_size) = 0;

		virtual bool __stdcall set_debuggee_process(unsigned long pid) = 0;
		virtual bool __stdcall set_debuggee_thread(unsigned long tid) = 0;
		virtual bool __stdcall get_context(void *context, size_t context_size) = 0;
		virtual bool __stdcall get_thread_context(cpu_context_type *context) = 0;

		virtual bool __stdcall write_file_log(wchar_t *log_dir, wchar_t *log_file_name, wchar_t *format, ...) = 0;
		virtual bool __stdcall write_binary(wchar_t *bin_dir, wchar_t *bin_file_name, unsigned char *dump, size_t size) = 0;
		virtual bool __stdcall read_binary(wchar_t *bin_dir, wchar_t *bin_file_name, unsigned char *dump, size_t size) = 0;

		virtual bool __stdcall get_process_table(void *table, size_t table_size, size_t *read_size) = 0;
		virtual void __stdcall clear_screen() = 0;
	};

	template <typename T1, class T2> bool __stdcall create(std::shared_ptr<T2> &u);
}
///
///
///
template <typename T1, class T2>
bool __stdcall engine::create(std::shared_ptr<T2> &u)
{
	try
	{
		void *o = nullptr;
		T1 *t = new T1;

		o = t;
		u.reset(static_cast<T2 *>(o));
	}
	catch (...)
	{
		return false;
	}

	return true;
}

#endif
