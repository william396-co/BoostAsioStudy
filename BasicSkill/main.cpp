#include <iostream>
#include <boost/asio.hpp>
#include <chrono>
#include <functional>
#include <thread>

void timer_asyn_test() {
	std::cout << __PRETTY_FUNCTION__ << "\n";


	boost::asio::io_context io;
	boost::asio::steady_timer t(io, std::chrono::seconds{ 5 });

	t.wait();
	std::cout << "Hello, Boost Asio\n";
}

void print(int const& code) {
	std::cout << __PRETTY_FUNCTION__ << " code :" << code << "\n";
}

void timer_async_wait_test() {
	std::cout << __PRETTY_FUNCTION__ << "\n";

	boost::asio::io_context io;
	boost::asio::steady_timer t(io, std::chrono::seconds{ 5 });

	t.async_wait(std::bind(&print,1));
	io.run();
}

void print2(boost::system::error_code error,boost::asio::steady_timer* t, int* count) {

	if (*count < 5) {
		std::cout << *count << std::endl;
		++(*count);

		t->expires_at(t->expiry() + std::chrono::seconds{ 1 });
		t->async_wait(std::bind(print2, boost::asio::placeholders::error, t, count));
	}
}

void bind_completion_handler_test() {
	std::cout << __PRETTY_FUNCTION__ << "\n";

	boost::asio::io_context io;
	boost::asio::steady_timer t(io, std::chrono::seconds{ 1 });
	int count = 0;
	t.async_wait(std::bind(print2, boost::asio::placeholders::error, &t, &count));
	io.run();

	std::cout << "Final Count is " << count << "\n";
}

constexpr auto max_count = 100;
class printer {
public:
	explicit printer(boost::asio::io_context& io):
		strand_{ boost::asio::make_strand(io) },
		timer1_{ io,std::chrono::seconds{1} },
		timer2_{ io,std::chrono::seconds{1} } {

		timer1_.async_wait(boost::asio::bind_executor(strand_,
			std::bind(&printer::print1, this)));

		timer2_.async_wait(boost::asio::bind_executor(strand_,
			std::bind(&printer::print2, this)));
	}
	~printer() {
		std::cout << "Final count is " << count_ << "\n";
	}
	void print1() {

		if (count_ < max_count) {
			std::cout << "timer 1: " << count_ << "\n";
			++count_;

			timer1_.expires_at(timer1_.expiry() + std::chrono::milliseconds{ 50 });
			timer1_.async_wait(boost::asio::bind_executor(strand_,
				std::bind(&printer::print1, this)));
		}

	}
	void print2() {
		if (count_ < max_count) {
			std::cout << "timer 1: " << count_ << "\n";
			++count_;

			timer2_.expires_at(timer1_.expiry() + std::chrono::milliseconds{ 50 });
			timer2_.async_wait(boost::asio::bind_executor(strand_,
				std::bind(&printer::print2, this)));
		}
	}
private:
	boost::asio::strand<boost::asio::io_context::executor_type> strand_;
	boost::asio::steady_timer timer1_;
	boost::asio::steady_timer timer2_;
	int count_{};
};

void multithread_handler() {
	boost::asio::io_context io;
	printer p(io);
	std::thread t([&] { io.run(); });
	io.run();
	t.join();
}

int main() {

	//timer_asyn_test();
	//timer_async_wait_test();
	//bind_completion_handler_test();
	multithread_handler();
	system("Pause");

	return 0;
}