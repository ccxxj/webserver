from wsgiref import headers
import requests

host = 'http://127.0.0.1:80/'

def send_http_request(method, path, headers):
	url = host + path
	server_response = requests.request(method, url, headers=headers)
	return server_response.status_code


def test_request_with_non_supported_method_returns_error():
	headers = {'content-length': '0'}
	assert send_http_request("bla", "", headers) == 400


def test_request_with_long_method_returns_error():
	headers = {'content-length': '0'}
	assert send_http_request("nonimplementedmethod", "", headers) == 501


def test_request_with_invalid_content_length():
	headers = {'content-length': 'a'}
	assert send_http_request("post", "", headers) == 400

# TODO: check this test
# def test_request_with_long_uri():
#     assert send_http_request("get", "http://localhost:80/V57gxvvkHGfQ+Y7mTnEcSQi/dsfjkhsdkfhdskfhsdkjfhds,fhsbcfcjshfsjf/dfk/sdjhgasjdgasjdgasjdgasjdgsadgasdgasjdgasjdhgasjdgsajdgufdfdjfgsdjdfgsdjdgfsdfgs/SlpcyUF45J5ZeMrRuERsdkjfsdfkjhjnjGRBjGwD8282QRoXOmE1308vB6kr6viaf5aK4NeK7MBpm4RZ5RkayYyM1MkzQs/ztPODpEPnMIg1UdwVTi2nfc92c7rdU4ADHU+N4yUNtI3+wmfFlMiPF14d8f6QM3sJSUwllkzmua9JiUAq14dxUt24huefy07USXRMW7D8oZ7tlPO91Yi4QNOaHpf6orWA2mVoMMuTNLvT6Xg7ZIfH/Qal80jhrza3qhNm9IEY9SetIczC9hy0hUaxUpgppzLSPuTf/Yw2OJnilpmobsylZzrzZe6/q5nRcbJonvtN8XhAFNt1bNN9czndbOz/3qmKabO22X0N9fBDjd5Mh+hIaZ4dXaa0vqLpjAV9POpqj3nYBqEKoiWcxRKfqf4BXujWE7OaiVOOxVVygyhNha/gC7muGvVJkz1PPlz/uaWAiNnH25EiKcC2PnktXYdkBdParUhQI/oexObbSDyqdmheCMLDTRpC1wU7ctzfnNpw9KXwxQDdVoEXO7XlLy7649zKRR0ICvqMdTHleiT9oEesSG2AmELwi2lBy46yeKaQIBynvDrZ8wFE1Jajb2DNs8tPdVcITBkqJDNN/gQUPi5s5Khs9PuMz3KdluZ9amPKzZytUriXqUNkKl+ueBVKZR3YT9nxFFCdnNyD7MQoicBCYS05rmE/sBH24X8u6JwYGIZ/dW3Su7GeEhu7LGps+qrR0M6Q/AOAeE21YS/4ztzFXqVC4HaWZIE1uBR6KagPLDIVtvF/NFAex8/IguDXRn5aIwbmpTLFBdyQtWjQGTHae+584FlpB4ye5AAx8wkLu8ZbNtzzGSxCYyEw2E0pNAGILgzHk0LhyN7lFWpM5oVcYx7Dl5nRH4SDYtmgK1WNqCnb8lrbd8aEP5gBu8GdHCd1iOOnaaWMgQV8gZQWr8N5QYNclOfNMUFii3gVCq6EbBAq78eyasRa1kRflwFj3WGa6+BpoDheUBCJi/XjiYcP6EEIJuj3bZkDsbmS5iKUyAMokmd+KuisyO+a78ZVgaQrXTulctM4Od2tk0k2yd4WqHUNm8JWhSwF+64gRV2evMFK79oOCg0YPxDZxoaly2AutKp3lWmwV/CxEG7Qe+wvOAg6Z2AB55zZrGOWKEc/cgMzZZ1RxvfmLiD2ajTDTkCFHf067o6PfrKnpe06jxvdOCFZE16lYiGgCIMpQEaRbzlGeSpJnfaPUhGz4PlClepwm9cGqaDC5Mz3aTJaOXVNE8H2uM3WsuFlm6497/8bZ8SrFA0BxNKwDSnW31WisF9FZZ7YvEx/mmRtNOmIRA4kBnHPId5nzBxTNfe0g2yTMtdEN/KuEVELXaDPf1DHnfkesPGSZuHcacZowpPwT+tlZfp5gNLnY9w9WI3KXygAtqmHuLW/Rvf1hTqF1qPX/AAGRp2oAQBQjhW9JWvlXvWnvTwDxkKY+GqSZgOMe0riGnTmfC7kmLpUDehzuApnpy47Of8q8V3k6dKVULUYFms9ls+l83rnf8yF/ipccU4EzR4w7p8aIqsBILEF3mIESmbdrkM5fjbWQdcr3UK8/ELFU2I6AgHV2iF7bHwW/E6SCP8wQd8z63C5pGWPcnfp7818URlA/g77Lj9XTU2jscq86tKQ4b1uZU3LXFRj4SmbYbS5qNKz5ftcdoAK8RNSBS7TKKQYKMjMmOrMhLjB6hLwGBeGXlovJo6L8X/pG+R4aCuUu+Uu0kyS2ajs2Fg+njJJhP8F4dRnMrTQWx8eRLlWolIUEzIWi4gjl8KRD8wD3tw1h4jESvtdfkhsdjfhskdfhskjdfhskdfhlsbcjshfjshf,h//sdjfgsdgfsakdfgaksdgfsdfkjghds,fhds,jfh/dsfdsf") == 414
# 


# Response Tests
def test_not_allowed_method():
	response = requests.get("http://localhost/method-error/")
	assert response.status_code == 405
	assert response.headers["Allow"] == "POST"
	assert response.headers["Content-Length"] == "2866"

# def test_directory_listing_off():
# 	response = requests.get("http://localhost/directory-listing-off/")
# 	assert response.status_code == 403

def test_directory_listing_on():
	response = requests.get("http://localhost/directory-listing-on/")
	assert response.status_code == 200

def	test_redirection_not_followed():
	response = requests.get("http://localhost/old/", allow_redirects=False)
	assert response.status_code == 301
	assert response.headers["Location"] == "http://localhost:80/redirect/301.html"

def	test_redirection_followed():
	response = requests.get("http://localhost/old/", allow_redirects=True)
	assert response.status_code == 200

def test_custom_not_found():
	response = requests.get("http://localhost/xyz")
	assert response.status_code == 404
	assert response.headers["Content-Length"] == "12631"

def test_specifiying_index_name():
	response = requests.get("http://localhost/max-body-error/")
	assert response.status_code == 200
	assert response.headers["Content-Length"] == "654"

def test_serving_png_img():
	response = requests.get("http://localhost/image.png")
	assert response.status_code == 200
	assert response.headers["Content-Length"] == "675118"
	assert response.headers["Content-Type"] == "image/png"

def test_serving_pdf():
	response = requests.get("http://localhost/Webserv.pdf")
	assert response.status_code == 200
	assert response.headers["Content-Length"] == "1315094"
	assert response.headers["Content-Type"] == "application/pdf"

def test_max_body_size():
	response = requests.post("http://localhost/max-body-error", data={"irem": 5})
	assert response.status_code == 413

def test_different_port_servers():
	response = requests.post("http://localhost:8080/", data={"irem": 5})
	assert response.status_code == 413
