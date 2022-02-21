const request = require("supertest");

describe("Sending VALID requests to the webserver", () => {
    it("Sending valid request without headers", async () => {
        const response = await request("localhost:80").get("/");
        expect(response.statusCode).toBe(0); //TODO: replace with 200
    });
});
