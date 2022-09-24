import json


req_data = {
    "version": "1.0",
    "ret": "0",
    "error_code": "0",
    "msg": "success",
    "is_inside": "yes"
}
req_body = json.dumps(req_data)
print("from dic to json string: " + req_body)

rsp_data = json.loads(req_body)
print("req[version]: " + rsp_data["version"])
rsp_data["version"] = "2.0"
print("req[version]: " + rsp_data["version"])
