#pragma once
#include <string>
#include <assert.h>
#include <vector>
namespace Storage
{
 
	class IObserver
	{
	public:
		virtual void StorageNotify(int count) = 0;
	};
	template <class T>
	class XJRingBuffer
	{
	public :
		class Iterator
		{
		public:

			Iterator(int count, XJRingBuffer* buf,bool isLast)
			{
				count_ = count;
				buf_ = buf;
				if(isLast)
					fstpos_ = (buf->writePos_+buf->capacity_ - count)% buf->capacity_;
				else
					fstpos_ = (buf->writePos_ + buf->capacity_ - buf->count_) % buf->capacity_;		
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
			XJRingBuffer* buf_;
		};
	public:
		XJRingBuffer(int capacity):buff_(capacity)
		{
			//buff_ = std::unique_ptr<T>(new T[capacity]);
			capacity_ = capacity;
			//buff_.resize(capacity);
		}
		~XJRingBuffer() {}

		void Attach(IObserver* observer,int obsrvCnt)
		{
			observer_ = observer;
			obsrvCnt_ = obsrvCnt;
		}

		bool Add(T val)
		{
			buff_[writePos_++%capacity_] = val;
			writePos_ = writePos_ % capacity_;
			if (count_ < capacity_)++count_;
			if (obsrvCnt_&& GetCount() >= obsrvCnt_)
			{
				assert(observer_ != nullptr);
				observer_->StorageNotify(GetCount());
			}
		}
		
		XJRingBuffer::Iterator  ReadLast(int count,bool isSave = false)
		{
			return Read(count, true, isSave);
		}
		XJRingBuffer::Iterator  ReadEarly(int count, bool isSave = false)
		{
			return Read(count, false, isSave);
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
		void Clear()
		{
			count_ = 0;
			writePos_ = 0;
			readPos_ = 0;
			obsrvCnt_ = 0;
		}
		T* Get(int idx)
		{ 
			return &buff_[(readPos_+idx)% capacity_];
		}
		void UpdateReadPos(int count)
		{
			readPos_ = (readPos_ + count) % capacity_;
		}
	private:
		XJRingBuffer::Iterator  Read(int count, bool isLast, bool isSave = false)
		{
			if (isSave && GetCount() < count)
				count = GetCount();
			else  if (!isSave && count_ < count)
				count = count_;
			return XJRingBuffer::Iterator(count, this, isLast);

		}
		std::vector<T> buff_;
		//std::unique_ptr<T> buff_;
		int capacity_;
		int count_;
		int writePos_;
		int readPos_;
		int obsrvCnt_;
		IObserver* observer_;
	};
}

