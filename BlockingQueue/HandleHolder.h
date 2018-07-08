#ifndef _HandleHolder_H
#define _HandleHolder_H

#include <windows.h>

/************************************************************************/
/* Class that holds HANDLES ensuring proper destruction                 */
/* It is design decision to make this class noncopyable.                */
/* It makes design much cleaner. If one wants to copy handle than one   */
/* can do hv.set(DuplicateHandle(....))                                 */
/************************************************************************/
namespace OpenThreads {

	class HandleHolder {
	private:
		HANDLE _handle;

		inline void close() {
			if (_handle != INVALID_HANDLE_VALUE) CloseHandle(_handle);
			_handle = INVALID_HANDLE_VALUE;
		};

		// copy constructor - disallow
		HandleHolder(const HandleHolder& rhs);

		// assignment operator - disallow
		HandleHolder& operator=(const HandleHolder& rhs);

	public:
		// constructor
		HandleHolder()
			:_handle(INVALID_HANDLE_VALUE)
		{};

		// constructor from HANDLE  
		explicit HandleHolder(HANDLE h)
			:_handle(h)
		{};

		// destructor - CloseHandle()
		~HandleHolder()
		{
			close();
		};

		// accessor    
		const HANDLE& get() const {
			return _handle;
		}

		// mutator
		void set(HANDLE h) {
			if (_handle != INVALID_HANDLE_VALUE) close();
			_handle = h;
		}

		operator bool() {
			return _handle != INVALID_HANDLE_VALUE && _handle != NULL;
		};

	};

} // namespace




#endif
