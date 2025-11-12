import json
import time
import jwt
import requests


# Note: Place your own serviceAccount.json in the root directory to run this script.
with open("serviceAccount.json") as f:
    sa = json.load(f)

now = int(time.time())
# create JWT
payload = {
    "iss": sa["client_email"],
    "scope": "https://www.googleapis.com/auth/datastore",
    "aud": "https://oauth2.googleapis.com/token",
    "iat": now,
    "exp": now + 3600
}

additional_headers = {"alg": "RS256", "typ": "JWT"}
signed_jwt = jwt.encode(payload, sa["private_key"], algorithm="RS256", headers=additional_headers)

# Request OAuth2 access token
r = requests.post("https://oauth2.googleapis.com/token",
                  data={
                      "grant_type": "urn:ietf:params:oauth:grant-type:jwt-bearer",
                      "assertion": signed_jwt
                  })

print(r.json())  # JSON contains "access_token"
