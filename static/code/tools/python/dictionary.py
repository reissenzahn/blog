
counts = {
  'A': 1,
  'B': 4,
  'C': 3,
}

counts['B'] = 2
print(counts['B'])

# get keys, values and items
print(counts.keys())    # 
print(counts.values())  # 
print(counts.items())   # 

# return default if key is missing
print(counts.get('B', default=0))  # 
print(counts.get('X', default=0))  # 

value  = <dict>.setdefault(key, default=None)   # Returns and writes default if key is missing.


<dict> = dict(<collection>)                     # Creates a dict from coll. of key-value pairs.

<dict> = dict(zip(keys, values))                # Creates a dict from two collections.
<dict> = dict.fromkeys(keys [, value])          # Creates a dict from collection of keys.
<dict>.update(<dict>)                           # Adds items. Replaces ones with matching keys.
value = <dict>.pop(key)                         # Removes item or raises KeyError.


{k for k, v in d.items() if v == value}    # Returns set of keys that point to the value.

{k: v for k, v in <dict>.items() if k in keys}  # Returns a dictionary, filtered by keys.
