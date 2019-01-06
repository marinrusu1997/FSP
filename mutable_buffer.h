#pragma once

template< class ByteType,auto size>
struct mutable_buffer final
{
	typedef std::array<ByteType, size>						buffer_type;
	typedef typename buffer_type::const_iterator			iterator;

	mutable_buffer() : buffer_(), offset_(0), current_size_(0)
	{}

	constexpr auto			current_size(decltype(size) bytes) noexcept { this->current_size_ = bytes; }

	constexpr auto			reset() noexcept
	{
		this->current_size_		= 0;
		this->offset_			= 0;
	}

	[[nodiscard]]
	constexpr auto&			buffer() noexcept { return buffer_; }

	[[nodiscard]]
	constexpr auto			begin_read_iter() const noexcept { return buffer_.begin() + offset_; }
	
	[[nodiscard]]
	constexpr auto			end_read_iter() const noexcept { return buffer_.begin() + current_size_; }

	constexpr auto			consume(iterator iter) noexcept
	{
		if ((offset_ = iter - buffer_.begin()) == current_size_)
			reset();
	}

	constexpr auto			can_read() const noexcept { return current_size_ != 0 ? true : false; }

private:
	buffer_type				buffer_;
	decltype(size)			current_size_;
	decltype(size)			offset_;
};