#pragma once
#include <string>
#include <assert.h>
namespace Storage
{
 
	// class IObserver
	// {
	// public:
	// 	virtual void Notify(int count) = 0;
	// };
	template <class T>
	class RingBuffer
	{
	public :
		class Iterator
		{
		public:

			Iterator(int count, RingBuffer* buf,int offset=0)
			{
				count_ = count;
				buf_ = buf;
				fstpos_ = buf->readPos_ + offset;
				pos_ = fstpos_;
			}
			T& First()
			{
				pos_ = fstpos_;
				return buf_->buff_[pos_++%buf_->capacity_];
			}
			T& Next()
			{
				return buf_->buff_[pos_++%buf_->capacity_];
			}
			bool IsEnd()
			{
				return (pos_- fstpos_) == count_;
			}
			void UpdateReadPos()
			{
				buf_->readPos_ = (fstpos_ + count_) % buf_->capacity_;
			}
		private:
			int count_;
			int pos_;
			int fstpos_;
			RingBuffer* buf_;
		};
	public:
		RingBuffer(int capacity)
		{
			buff_ = std::unique_ptr<T>(new T[capacity]);
			capacity_ = capacity;
		}
		~RingBuffer() {}

		void Attach(IObserver* observer,int obsrvCnt)
		{
			// observer_ = observer;
			// obsrvCnt_ = obsrvCnt;
		}

		bool Add(T val)
		{
			buff_[writePos_++%capacity_] = val;
			writePos_ = writePos_ % capacity_;
			if (obsrvCnt_&& GetCount() >= obsrvCnt_)
			{
				// assert(observer_ != nullptr);
				// observer_->Notify(GetCount());
			}
		}
		RingBuffer::Iterator  ReadLast(int count)
		{
			if (GetCount() < count)
			{
				count = GetCount();
				return RingBuffer::Iterator(count, this);
			}			
			else
			{
				return RingBuffer::Iterator(count, this, GetCount()-count);
			}	
		}
		RingBuffer::Iterator  ReadEarly(int count)
		{
			if (GetCount() < count)
			{
				count = GetCount();	
			}
			return RingBuffer::Iterator(count, this);
		}
		int  GetCount()
		{
			return (writePos_ + capacity_ - readPos_) % capacity_;
		}
		int  GetFreeCount()
		{
			return capacity_- GetCount();
		}
		bool IsEmpty()
		{
			return writePos_ == readPos_;
		}
		bool IsFull()
		{
			return GetFreeCount()==0;
		}

		T* Get(int idx)
		{
 
			return &buff_.get()[(readPos_+idx)% capacity_];
		}
	private:
		std::unique_ptr<T> buff_;
		int capacity_;
		int count_;
		int writePos_;
		int readPos_;
		int obsrvCnt_;
		// IObserver* observer_;
	};
}

