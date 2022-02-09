#include "StatusCodes.hpp"

#include <stdexcept>

namespace HTTPResponse {

	const std::string get_reason_phrase(StatusCode code) {

		switch (code)
		{
			// 1xx - Informational 
			case Continue: return "Continue";
			case SwitchingProtocols: return "Switching Protocols";
			case Processing: return "Processing";
			case EarlyHints: return "Early Hints";
			// 2xx - Successful 
			case OK: return "OK";
			case Created: return "Created";
			case Accepted: return "Accepted";
			case NonAuthoritativeInformation: return "Non-Authoritative Information";
			case NoContent: return "No Content";
			case ResetContent: return "Reset Content";
			case PartialContent: return "Partial Content";
			case MultiStatus: return "Multi-Status";
			case AlreadyReported: return "Already Reported";
			case IMUsed: return "IM Used";

			// 3xx - Redirection 
			case MultipleChoices: return "Multiple Choices";
			case MovedPermanently: return "Moved Permanently";
			case Found: return "Found";
			case SeeOther: return "See Other";
			case NotModified: return "Not Modified";
			case UseProxy: return "Use Proxy";
			case TemporaryRedirect: return "Temporary Redirect";
			case PermanentRedirect: return "Permanent Redirect";

			// 4xx - Client Error 
			case BadRequest: return "Bad Request";
			case Unauthorized: return "Unauthorized";
			case PaymentRequired: return "Payment Required";
			case Forbidden: return "Forbidden";
			case NotFound: return "Not Found";
			case MethodNotAllowed: return "Method Not Allowed";
			case NotAcceptable: return "Not Acceptable";
			case ProxyAuthenticationRequired: return "Proxy Authentication Required";
			case RequestTimeout: return "Request Timeout";
			case Conflict: return "Conflict";
			case Gone: return "Gone";
			case LengthRequired: return "Length Required";
			case PreconditionFailed: return "Precondition Failed";
			case ContentTooLarge: return "Content Too Large";
			case URITooLong: return "URI Too Long";
			case UnsupportedMediaType: return "Unsupported Media Type";
			case RangeNotSatisfiable: return "Range Not Satisfiable";
			case ExpectationFailed: return "Expectation Failed";
			case ImATeapot: return "I'm a teapot";
			case MisdirectedRequest: return "Misdirected Request";
			case UnprocessableContent: return "Unprocessable Content";
			case Locked: return "Locked";
			case FailedDependency: return "Failed Dependency";
			case TooEarly: return "Too Early";
			case UpgradeRequired: return "Upgrade Required";
			case PreconditionRequired: return "Precondition Required";
			case TooManyRequests: return "Too Many Requests";
			case RequestHeaderFieldsTooLarge: return "Request Header Fields Too Large";
			case UnavailableForLegalReasons: return "Unavailable For Legal Reasons";

			// 5xx - Server Error 
			case InternalServerError: return "Internal Server Error";
			case NotImplemented: return "Not Implemented";
			case BadGateway: return "Bad Gateway";
			case ServiceUnavailable: return "Service Unavailable";
			case GatewayTimeout: return "Gateway Timeout";
			case HTTPVersionNotSupported: return "HTTP Version Not Supported";
			case VariantAlsoNegotiates: return "Variant Also Negotiates";
			case InsufficientStorage: return "Insufficient Storage";
			case LoopDetected: return "Loop Detected";
			case NotExtended: return "Not Extended";
			case NetworkAuthenticationRequired: return "Network Authentication Required";

			default: throw std::invalid_argument("Invalid status code.");
		}
	}
}
