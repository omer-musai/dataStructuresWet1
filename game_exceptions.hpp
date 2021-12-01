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

		explicit Exception(const std::string& error_type) : error_type(error_type)
		{
			this->error_type = "An error has occurred: " + error_type;
		}
		
		const char* what() const noexcept override {
			return error_type.c_str();
		}
	private:
		std::string error_type;
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
