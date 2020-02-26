#ifndef RWLOCK_H
#define RWLOCK_H

#include <cstdlib>
#include <cassert>
#include <atomic>
#include <thread>
#include "raii.h"

class RWLock
{
#define WRITE_LOCK_STATUS -1
#define FREE_STATUS 0
private:
	static const std::thread::id NULL_THREAD;
	const bool WRITE_FIRST;
	std::thread::id m_write_thread_id;
	std::atomic_int m_lockCount;
	std::atomic_int m_writeWaitCount;
public:
	RWLock( const RWLock& ) = delete;
	RWLock operator = ( const RWLock& ) = delete;
	RWLock( bool writeFirst = false );
	virtual ~RWLock() = delete;
	int readLock();
	int readUnlock();
	int writeLock();
	int writeUnlock();

	raii read_guard() const noexcept
	{
		return make_raii( *this, &RWLock::readUnlock, &RWLock::readLock );
	}

	raii write_guard() noexcept
	{
		return make_raii( *this, &RWLock::writeUnlock, &RWLock::writeLock );
	}
};

#endif // RWLOCK_H
