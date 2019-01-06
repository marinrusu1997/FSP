#pragma once

#include <memory>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/chrono.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/function.hpp>

/// Simple asio::deadline_timer wrapper to aggregate
/// timer, interval and handler function.
class simple_timer final : public std::enable_shared_from_this<simple_timer>
{
	typedef boost::asio::deadline_timer impl;
public:
	typedef impl::duration_type duration;
	typedef boost::function<void(boost::system::error_code const&)> handler_function;

	/// Create timer with specified interval and handler
	simple_timer(boost::asio::io_context& io_context, handler_function handler)
		: impl_(io_context)
		, handler_(std::move(handler))
	{
	}

	simple_timer(boost::asio::io_context& io_context) :
		impl_(io_context)
	{}

	void set_handler(handler_function&& func) { handler_ = std::move(func); }

	/// Start asynchronous waiting
	void start(duration duration)
	{
		this->duration_ = duration;
		impl_.expires_from_now(duration);
		impl_.async_wait(handler_);
	}

	/// Restart async waiting with previous duration
	void restart()
	{
		stop();
		start(this->duration_);
	}

	/// Stop waiting
	void stop()
	{
		impl_.cancel();
	}

	operator bool() const
	{
		return impl_.expires_at() <= boost::asio::deadline_timer::traits_type::now();
	}

private:
	impl				impl_;
	duration			duration_;
	handler_function	handler_;
};


