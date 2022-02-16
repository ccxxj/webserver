const request = require("supertest");

describe("webserv test", () => {
    it("tests /destinations endpoints", async () => {
        const response = await request("localhost:80")
            .get("questions/3091485/what-is-the-limiquestioquestions")
            .set("X-API-Key", " foobar")
            .set("Accept", "application/json");
        expect(response.statusCode).toBe(0); //TODO: replace with 200
    });
});
