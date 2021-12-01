#ifndef GAME_EXCEPTIONS_H
#define GAME_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class Exception : public std::exception
{
	public:
		Exception() = delete;
		Exception(const Exception& other) = default;
		virtual ~Exception() noexcept = default;

		explicit Exception(std::string& error_message) : error_type(error_type)
		{
			error_message = (error_type);
		}
		
		const char* what() const noexcept override {
			return error_message.c_str();
		}
	private:
		std::string error_type;
		std::string error_message;
};

class AllocationError : public Exception
{
	public:
		explicit AllocationError() : Exception("AllocationError") {}

};


class InvalidInput : public Exception
{
	public:
		explicit InvalidInput() : Exception("InvalidInput") {}

};

class Failure : public Exception
{
	public:
		explicit Failure() : Exception("Failure") {}
};

#endif //GAME_EXCEPTIONS_H
