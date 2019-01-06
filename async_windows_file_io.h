#ifndef _ASYNC_FILE_IO_WINDOWS_HEADER_
#define _ASYNC_FILE_IO_WINDOWS_HEADER_

#include <boost/asio/read.hpp>
#include <boost/asio/write_at.hpp>
#include <boost/asio/read_at.hpp>
#include <boost/asio/windows/random_access_handle.hpp>

#include <Windows.h>

namespace boost::asio {
	class io_context;
}

namespace detail {
	/// @brief Handler to wrap asynchronous read_some_at operations.
	template <typename Handler>
	class read_write_some_offset_handler
	{
	public:
		read_write_some_offset_handler(Handler handler, boost::uint64_t& offset)
			: handler_(handler),
			offset_(offset)
		{}

		void operator()(
			const boost::system::error_code& error,
			std::size_t bytes_transferred)
		{
			offset_ += bytes_transferred;

			const boost::system::error_code result_ec =
				(error && bytes_transferred)
				? make_error_code(boost::system::errc::success) : error;
			//std::cout << "Offset " << offset_ << "Bytes Transferred " << bytes_transferred << std::endl;
			handler_(result_ec, bytes_transferred);
		}

		auto& handler() { return handler_; }

		Handler				handler_;
		boost::uint64_t&	offset_;
	};

	/// @brief Hook that allows the wrapped handler to be invoked
	///        within specific context.  This is critical to support
	///        composed operations being invoked within a strand.
	template <typename Function,
		typename Handler>
		void asio_handler_invoke(
			Function function,
			detail::read_write_some_offset_handler<Handler>* handler)
	{
		boost_asio_handler_invoke_helpers::invoke(function, handler->handler_);
	}
} // namespace detail

/// @brief Adapts AsyncRandomAccessReadDevice to support AsyncReadStream.
template <typename AsyncRandomAccessReadDevice>
class basic_adapted_stream
	: public AsyncRandomAccessReadDevice
{
public:
	basic_adapted_stream(
		boost::asio::io_context& io_context,
		HANDLE handle
	)
		: AsyncRandomAccessReadDevice(io_context, handle),
		read_offset_(0),
		write_offset_(0)
	{}

	template<typename MutableBufferSequence,
		typename ReadHandler>
		void async_read_some(
			const MutableBufferSequence& buffers,
			ReadHandler handler)
	{
		async_read_at(*this, read_offset_, buffers,
			detail::read_write_some_offset_handler<ReadHandler>(handler, read_offset_));
	}

	template<typename ConstBufferSequence,
		typename WriteHandler>
		void async_write_some(
			const ConstBufferSequence& buffers,
			WriteHandler handler)
	{
		async_write_at(*this, write_offset_, buffers,
			detail::read_write_some_offset_handler<WriteHandler>(handler, write_offset_));
	}

private:
	boost::uint64_t read_offset_;
	boost::uint64_t write_offset_;
};

#ifndef ADAPT_IO_SERVICE
typedef basic_adapted_stream<
	boost::asio::windows::random_access_handle> adapted_stream;
#else
typedef boost::asio::windows::basic_stream_handle<
	offset_stream_handle_service> adapted_stream;
#endif

class AsyncWindowsFileIO
{
public:
	AsyncWindowsFileIO(boost::asio::io_context& io_context, HANDLE handle)
		: io_context_(io_context),
		io_handle(io_context, handle)
	{}

	template <typename ReadHandler, typename MutableBuffer>
	void async_read(ReadHandler handler, MutableBuffer buffer)
	{
		boost::asio::async_read(io_handle, buffer, handler);
	}

	template <typename WriteHandler, typename ConstBuffer>
	void async_write(WriteHandler handler, ConstBuffer buffer)
	{
		boost::asio::async_write(io_handle, buffer, handler);
	}

	void close() { boost::system::error_code ec; io_handle.close(ec); }

private:
	boost::asio::io_context&	io_context_;
	adapted_stream				io_handle;
};

#endif // !_ASYNC_FILE_IO_WINDOWS_HEADER_

