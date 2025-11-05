# SocialNet Simulator

## 1. Project Overview

This project implements a command-line social network backend simulator using custom data structures. The system supports user management, mutual friendships, timestamped content posting, friend recommendations based on mutual connections, and shortest-path queries for degrees of separation.

**Core Features:**
- AVL tree implementation for timestamp-keyed posts enabling O(log n) insertions and efficient retrieval of most recent content
- Graph-based friendship network using hash containers for O(1) average-case lookups
- BFS-based shortest-path computation for degrees of separation
- Mutual-friends algorithm for intelligent friend suggestions
- Color-coded terminal output for enhanced user experience

**Supported Commands:**  
`ADD_USER`, `ADD_FRIEND`, `LIST_FRIENDS`, `SUGGEST_FRIENDS`, `ADD_POST`, `OUTPUT_POSTS`, `DEGREES_OF_SEPARATION`, `EXIT`

**Note:** All usernames are canonicalized to lowercase internally, making the interface case-insensitive.

---

## 2. File Structure and Explanations

| File | Purpose |
|------|---------|
| `main.cpp` | Command-line shell that reads and parses commands, validates syntax, and dispatches operations to `SocialNet`. Handles colored terminal output and displays startup banner. |
| `social_net.hpp` | Implements the `SocialNet` class. Manages an `unordered_map<string, User>` for O(1) user lookup and orchestrates friendship operations, friend suggestions, and degrees-of-separation queries. |
| `user.hpp` | Defines the `User` class, storing lowercase username, an `AVLTree` instance for posts, and an `unordered_set<string>` of friend usernames. |
| `AVL_tree.hpp` | AVL tree implementation for order-keyed posts, supporting posts with time. Supports insertion, deletion by order of post, and reverse in-order traversal for chronologically-ordered post retrieval. |
| `build.sh` | Shell script to compile the project using g++/clang++. |

---

## 3. Compilation Instructions

Ensure you are in a UNIX or UNIX-like environment (use GitBash or WSL on Windows).

**(One Time) Make the build script executable:**
```bash
chmod +x build.sh
```

**Compile the project:**
```bash
./build.sh
```

**Requirements:**
- g++ (or clang++) with at least C++11 support
- Tested on macOS with Apple clang++ version 15.0.0

The script compiles `main.cpp` and produces an executable called `socialnet`.

---

## 4. Running the Program

Execute the compiled binary:
```bash
./socialnet
```

**Input Modes:**

*Interactive mode:*
```bash
./socialnet
ADD_USER Alice
ADD_FRIEND Alice Bob
```

*Batch mode with input file:*
```bash
./socialnet < testcases.txt
```

**Output Color Coding:**
- **Green:** Successful operations and informational messages
- **Yellow:** Warnings (e.g., duplicate user, non-existent friend)
- **Red:** Errors (e.g., invalid syntax, unknown command)

---

## 5. Supported Commands and Syntax

### 5.1 Social Network Operations

| Command | Syntax | Description |
|---------|--------|-------------|
| `ADD_USER` | `ADD_USER <username>` | Creates a new user with the specified username (converted to lowercase). Initially has no friends or posts. |
| `ADD_FRIEND` | `ADD_FRIEND <username1> <username2>` | Establishes a bidirectional friendship between two existing users. |
| `LIST_FRIENDS` | `LIST_FRIENDS <username>` | Prints an alphabetically-sorted list of the specified user's friends. |
| `SUGGEST_FRIENDS` | `SUGGEST_FRIENDS <username> [N]` | Recommends up to N non-friend users ranked by number of mutual friends (ties broken alphabetically). If N is omitted, all friend suggestions are shown. N must be -1 (for all) or a non-negative integer.|
| `DEGREES_OF_SEPARATION` | `DEGREES_OF_SEPARATION <username1> <username2>` | Computes the shortest friendship path length between two users using BFS. Returns -1 if no path exists. |

### 5.2 User Content Operations

| Command | Syntax | Description |
|---------|--------|-------------|
| `ADD_POST` | `ADD_POST <username> "<content>"` | Adds a timestamped post for the specified user. Content is extracted from remainder of line; surrounding quotes are stripped. Post is keyed by the order of the post in which it is entered. |
| `OUTPUT_POSTS` | `OUTPUT_POSTS <username> <N>` | Displays the N most recent posts in reverse chronological order. Use N = -1 to display all posts. |


### 5.3 System Commands

| Command | Description |
|---------|-------------|
| `EXIT` | Terminates the simulator. |

---

## 6. Error and Edge Case Handling

The system provides clear, color-coded error messages for all invalid inputs and operations:

**User Management Errors:**
- Duplicate user addition → Yellow warning: `"User '<username>' already exists"`
- Non-existent user reference → Yellow/Red: `"User '<username>' not found"`
- Self-friendship attempt → Yellow warning: `"Cannot add self as friend"`

**Friendship Errors:**
- Already friends → Yellow warning: `"Users '<u1>' and '<u2>' are already friends"`
- Missing user in friendship operation → Red error with details

**Content Errors:**
- Empty post content → Red error: `"Invalid syntax. Post content cannot be empty"`
- Post to non-existent user → Yellow error with username

**Output/Post Query Errors (OUTPUT_POSTS):**
- Missing N argument (e.g. `OUTPUT_POSTS alice`) → Red error: usage message requiring both username and N.
- Non-integer N (e.g. `OUTPUT_POSTS alice two`) → Red error: N must be an integer.
- N < -1 (e.g. `OUTPUT_POSTS alice -2`) → Yellow warning: invalid N; must be -1 or non-negative.
- User has no posts → Yellow warning: `User <name> has no posts.`

**Query Errors (SUGGEST_FRIENDS):**
- Missing N parameter (e.g., `SUGGEST_FRIENDS Alice`) → Treated as request for all suggestions (N=-1 default).
- Non-integer N (e.g., `SUGGEST_FRIENDS Alice abc`) → Red error: N must be a valid integer.
- N < -1 (e.g., `SUGGEST_FRIENDS Alice -2`, `SUGGEST_FRIENDS Alice -5`) → Yellow warning: N must be -1 (for all) or non-negative.
- User has no potential friend suggestions → Yellow warning: `No friend suggestions available for <username>.`

**Query Errors (DEGREES_OF_SEPARATION):**
- Missing username argument → Red error: Two usernames required.
- One or both users do not exist → Yellow warning: One or both users do not exist.
- No path exists between users → Returns -1.

**System Errors:**
- Unknown command → Red error: `"Unknown command: '<command>'"`
- Insufficient arguments → Red error with usage information

---

## 7. Design Choices and Assumptions

**Username Canonicalization:**  
All usernames are converted to lowercase at input time and stored in lowercase in `User::username` and the `users` map. This ensures case-insensitive comparisons throughout the system.

**Friendship Storage:**  
The `unordered_set<string>` data structure stores friend relationships, providing expected O(1) insertion and lookup. Friend lists are sorted only for display purposes.

**Post Timestamping:**  
Posts are stored in an AVL tree keyed by the order the posts are inserted. They also include the `time_t` timestamp (obtained via `time(nullptr)` at insertion). This maintains chronological ordering and guarantees O(log P) insertion and O(N) retrieval for the N most recent posts via reverse in-order traversal.

**Friend Suggestion Algorithm:**  
Counts mutual friends by iterating over the target user's friends and aggregating their friends into a frequency map. See Section 8 for complexity analysis and proof sketches. If no number is provided, it outputs all possible friend suggestions.

**Degrees of Separation:**  
Standard breadth-first search (BFS) on an implicit adjacency list derived from `User::friends`.

**Input Assumptions:**  
The following assumptions are made about input format and behavior (as recommended by course staff):

- **Usernames:** Must be single words without spaces. Space-delimited parsing is used, so `ADD_USER John Doe` would be interpreted as invalid input.
- **Post content:** Can contain spaces. Content is extracted from the remainder of the line after the username.
- **Command syntax:** Commands are case-sensitive and must be uppercase (e.g., `ADD_USER` not `add_user` or `Add_User`).
- **Username case-insensitivity:** Usernames are case-insensitive. `Alice`, `alice`, and `ALICE` refer to the same user.
- **Post content case:** Preserved exactly as entered. Posts are not converted to lowercase.
- **Duplicate posts:** Allowed. The same content can be posted multiple times.
- **Timestamp collisions:** Posts created within the same second have identical timestamps. Due to AVL tree duplicate key handling, only the first post in that second is stored. For production use, higher-resolution timestamps would be needed.
- **Empty results:** `LIST_FRIENDS` for a user with no friends prints the header but no names. `SUGGEST_FRIENDS` with no candidates prints a warning message.
- **N parameter in SUGGEST_FRIENDS:**
  - If N is omitted (e.g., `SUGGEST_FRIENDS Alice`), all friend suggestions are displayed (treated as N=-1 internally).
  - N must be -1 (for all suggestions) or a non-negative integer (0, 1, 2, ...). 
  - Negative values less than -1 (e.g., `-2`, `-5`, `-100`) trigger a yellow warning: "N must be -1 (for all) or non-negative."
  - Non-integer values (e.g., `abc`, `3.14`, `two`) trigger a red error: "N must be a valid integer."
  - N=0 displays no suggestions (outputs nothing, returns silently by design).
  - If there are fewer candidates than N, all available candidates are shown.
- **Quotes in ADD_POST:** Input will not contain surrounding quotes per course staff clarification. The code defensively strips quotes if present.
- **Special characters:** Usernames should contain only alphanumeric characters. Special characters may cause undefined behavior.
- **Input validation:** Basic validation is performed (missing arguments trigger errors). The system assumes otherwise well-formed input.
- **Repeated operations:** Duplicate `ADD_USER` warns that username exists. Repeated `ADD_FRIEND` for same pair warns they are already friends.
- **Self-friendship:** Attempting `ADD_FRIEND Alice Alice` is rejected with a warning.
- **Non-existent users:** Operations on non-existent users produce appropriate error messages.

---

## 8. Complexity Analysis

### 8.1 Notation

- **U** = number of users (|users|)
- **F(u)** = number of friends of user u
- **F** = average/maximum number of friends per user
- **P(u)** = number of posts by user u
- **P** = average/maximum number of posts per user
- **V** = number of vertices in friendship graph = U
- **E** = number of edges (mutual friendships, counted once)

### 8.2 Operation Complexities

| Operation | Dominant Step | Time Complexity | Proof Sketch |
|-----------|---------------|-----------------|--------------|
| `ADD_USER` | Hash map insertion | **O(1)** expected | Hash map insertion averages constant time with good hash distribution. Constructs a `User` and performs `users[name] = u`. |
| `ADD_FRIEND` | Two set insertions | **O(1)** expected | Each `unordered_set` insertion is O(1) average. Updating both friendship sets requires two constant-time operations. |
| `LIST_FRIENDS` | Sorting friends | **O(F log F)** | Converts `unordered_set` to `vector` of size F and sorts. Standard sort is O(F log F). |
| `ADD_POST` | AVL insertion | **O(log P)** worst-case | AVL insertion performs BST insert plus at most O(1) rotations. Tree height is O(log P), so insertion is O(log P). |
| `OUTPUT_POSTS` | Reverse in-order traversal | **O(N)** | Reverse in-order visits each of the top N nodes exactly once. Linear in N. |
| `SUGGEST_FRIENDS` | Counting + sorting | **O(F² + C log C)** ≈ **O(F² log F)** worst-case | For user u with F friends, iterate over every friend f and their F(f) friends. Dense case: F(f) ≈ F yields O(F²) counting. Sorting C candidates (C ≤ U) costs O(C log C). Worst-case simplified to O(F² log F). |
| `DEGREES_OF_SEPARATION` | BFS traversal | **O(V + E)** | Standard BFS visits each vertex once and examines each edge twice (undirected). Total: O(V + E). |

### 8.3 Detailed Proof Sketches

**AVL Insertion (O(log P)):**  
AVL tree height h satisfies h ≤ 1.44 log₂(P + 2) (standard bound). Traversal to insertion point is O(h), and rebalancing requires at most O(1) rotations per level. Therefore, insertion is O(log P).

**BFS for Degrees of Separation (O(V + E)):**  
Each vertex is enqueued and dequeued at most once. Each edge is examined at most twice (once from each endpoint in an undirected graph). Total work is Θ(V + E).

**Friend Suggestion (O(F² log F)):**  
For target user u with F friends, iterate over each friend f and their friend list. If each friend has up to F members, pairwise operations yield O(F²) counting. The candidate vector size C ≤ U; sorting C items requires O(C log C). Combined bound: O(F² + C log C) ≈ O(F² log F) worst-case.

---

## 9. Sample Interaction

**Input:**
```
ADD_USER Alice
ADD_USER Bob
ADD_USER Charlie
ADD_FRIEND Alice Bob
ADD_FRIEND Bob Charlie
LIST_FRIENDS Bob
ADD_POST Alice "Hello World from SocialNet"
OUTPUT_POSTS Alice 1
SUGGEST_FRIENDS Alice 3
DEGREES_OF_SEPARATION Alice Charlie
EXIT
```

**Expected Output:**
```
User alice added successfully.
User bob added successfully.
User charlie added successfully.
Users alice and bob are now friends.
Users bob and charlie are now friends.
The friends of bob are the following:
alice charlie
Post added for user alice.
Wed Nov  5 01:47:45 2025: Hello World from SocialNet
Friend suggestions for alice:
charlie (1 mutual)
2
Exiting SocialNet Simulator. Goodbye!
```

**Notes:**
- Usernames are canonicalized to lowercase in all output
- Timestamps are generated by `ctime()` and will vary based on execution time
- The degree of separation between Alice and Charlie is 2 (Alice → Bob → Charlie)

---

## 10. Comprehensive Test Suite

Create a `testcases.txt` file and execute with input redirection:
```bash
./socialnet < testcases.txt
```

The test suite is organized into six phases:
1. **User and Friendship Management** - Basic operations, duplicate handling, self-friendship
2. **Content Management (AVL Tree)** - Post creation, retrieval, chronological ordering
3. **Friend Suggestions and Graph Queries** - BFS pathfinding, mutual friend recommendations
4. **Friend Suggestion Edge Cases** - Comprehensive N parameter validation
5. **Invalid Inputs and Edge Cases** - Error handling across all commands
6. **Stress Testing (Optional)** - Large network performance

### Phase 1: User and Friendship Management

```
ADD_USER Alice
ADD_USER Bob
ADD_USER Charlie
ADD_USER Dave
ADD_USER Eve
ADD_USER Alice
ADD_FRIEND Alice Bob
ADD_FRIEND Alice Charlie
ADD_FRIEND Bob Charlie
ADD_FRIEND Eve Bob
ADD_FRIEND Eve Eve
ADD_FRIEND Alice NonExistent
LIST_FRIENDS Alice
LIST_FRIENDS Bob
LIST_FRIENDS NonExistent
```

**Expected Behavior:**
- First 5 users added successfully
- Duplicate Alice addition triggers warning
- Friendships established (Alice-Bob, Alice-Charlie, Bob-Charlie, Eve-Bob)
- Self-friendship (Eve-Eve) rejected with warning
- Friendship with non-existent user rejected with error
- Friend lists displayed alphabetically

---

### Phase 2: Content Management (AVL Tree)

```
ADD_POST Alice "First post - testing the system"
ADD_POST Alice "Second post - AVL tree working"
ADD_POST Alice "Third post - chronological order test"
ADD_POST Bob "Bob's first post"
OUTPUT_POSTS Alice 2
OUTPUT_POSTS Alice -1
OUTPUT_POSTS Bob 1
OUTPUT_POSTS NonExistent 1
ADD_POST Charlie ""
```

**Expected Behavior:**
- Posts added with timestamps
- OUTPUT_POSTS Alice 2 shows two most recent posts in reverse chronological order
- OUTPUT_POSTS Alice -1 shows all three posts
- Empty post content rejected with error

---

### Phase 3: Friend Suggestions and Graph Queries

```
ADD_USER Frank
ADD_USER Grace
ADD_FRIEND Bob Dave
ADD_FRIEND Charlie Dave
ADD_FRIEND Dave Frank
ADD_FRIEND Frank Grace
SUGGEST_FRIENDS Alice 5
SUGGEST_FRIENDS Dave 3
SUGGEST_FRIENDS Grace 10
DEGREES_OF_SEPARATION Alice Dave
DEGREES_OF_SEPARATION Alice Grace
DEGREES_OF_SEPARATION Alice Frank
DEGREES_OF_SEPARATION Eve Grace
DEGREES_OF_SEPARATION NonExistent Alice
```

**Expected Behavior:**
- Extended network: Alice-Bob-Charlie-Dave-Frank-Grace chain with Eve connected to Bob
- SUGGEST_FRIENDS Alice 5 should suggest Dave (2 mutual: Bob, Charlie) and Eve (1 mutual: Bob)
- Degrees of separation computed via BFS
- Alice to Grace should show path length (3 or 4 depending on route)
- Non-existent user queries trigger errors

---

### Phase 4: Friend Suggestion Edge Cases

```
ADD_USER TestUser1
ADD_USER TestUser2
ADD_USER TestUser3
ADD_USER TestUser4
ADD_FRIEND TestUser1 TestUser2
ADD_FRIEND TestUser2 TestUser3
ADD_FRIEND TestUser3 TestUser4
SUGGEST_FRIENDS TestUser1
SUGGEST_FRIENDS TestUser1 -1
SUGGEST_FRIENDS TestUser1 0
SUGGEST_FRIENDS TestUser1 1
SUGGEST_FRIENDS TestUser1 5
SUGGEST_FRIENDS TestUser1 -2
SUGGEST_FRIENDS TestUser1 -100
SUGGEST_FRIENDS TestUser1 abc
SUGGEST_FRIENDS TestUser1 3.14
SUGGEST_FRIENDS TestUser1 two
SUGGEST_FRIENDS NonExistentUser 3
```

**Expected Behavior:**
- TestUser1 → TestUser2 → TestUser3 → TestUser4 chain
- `SUGGEST_FRIENDS TestUser1` (no N) → Shows TestUser3 (1 mutual: TestUser2)
- `SUGGEST_FRIENDS TestUser1 -1` → Shows all suggestions (same as above)
- `SUGGEST_FRIENDS TestUser1 0` → Outputs nothing (silent return)
- `SUGGEST_FRIENDS TestUser1 1` → Shows top 1 suggestion (TestUser3)
- `SUGGEST_FRIENDS TestUser1 5` → Shows all available (only TestUser3, fewer than 5)
- `SUGGEST_FRIENDS TestUser1 -2` → Yellow warning: N must be -1 or non-negative
- `SUGGEST_FRIENDS TestUser1 -100` → Yellow warning: N must be -1 or non-negative
- `SUGGEST_FRIENDS TestUser1 abc` → Red error: N must be a valid integer
- `SUGGEST_FRIENDS TestUser1 3.14` → Red error: N must be a valid integer
- `SUGGEST_FRIENDS TestUser1 two` → Red error: N must be a valid integer
- `SUGGEST_FRIENDS NonExistentUser 3` → Yellow warning: User does not exist

---

### Phase 5: Invalid Inputs and Edge Cases

```
ADD_USER
ADD_FRIEND Alice
ADD_FRIEND Alice Alice
ADD_POST
ADD_POST Alice
OUTPUT_POSTS Alice
OUTPUT_POSTS Alice two
OUTPUT_POSTS Alice -2
ADD_USER NoPostsUser
OUTPUT_POSTS NoPostsUser 1
SUGGEST_FRIENDS Alice
SUGGEST_FRIENDS Alice abc
SUGGEST_FRIENDS Alice -2
SUGGEST_FRIENDS Alice -5
SUGGEST_FRIENDS Alice 0
SUGGEST_FRIENDS Alice 3.14
SUGGEST_FRIENDS NonExistentUser 5
DEGREES_OF_SEPARATION Alice
RANDOM_COMMAND
UnknownCommand with arguments
EXIT
```

**Expected Behavior:**
- All commands with missing/insufficient arguments trigger usage errors
- Self-friendship (Alice-Alice) rejected with yellow warning
- **OUTPUT_POSTS edge cases:**
  - `OUTPUT_POSTS Alice` → Red error: requires both username and N
  - `OUTPUT_POSTS Alice two` → Red error: N must be an integer
  - `OUTPUT_POSTS Alice -2` → Yellow warning: N must be -1 or non-negative
  - `OUTPUT_POSTS NoPostsUser 1` → Yellow warning: User has no posts
- **SUGGEST_FRIENDS edge cases:**
  - `SUGGEST_FRIENDS Alice` → Shows all friend suggestions (N=-1 default)
  - `SUGGEST_FRIENDS Alice abc` → Red error: N must be a valid integer
  - `SUGGEST_FRIENDS Alice -2` → Yellow warning: N must be -1 or non-negative
  - `SUGGEST_FRIENDS Alice -5` → Yellow warning: N must be -1 or non-negative
  - `SUGGEST_FRIENDS Alice 0` → Outputs nothing (by design)
  - `SUGGEST_FRIENDS Alice 3.14` → Red error: N must be a valid integer
  - `SUGGEST_FRIENDS NonExistentUser 5` → Yellow warning: User does not exist
- Unknown commands trigger red error messages
- System exits cleanly on EXIT

---

### Phase 6: Stress Testing (Optional)

```
ADD_USER User1
ADD_USER User2
ADD_USER User3
ADD_USER User4
ADD_USER User5
ADD_USER User6
ADD_USER User7
ADD_USER User8
ADD_USER User9
ADD_USER User10
ADD_FRIEND User1 User2
ADD_FRIEND User1 User3
ADD_FRIEND User2 User4
ADD_FRIEND User2 User5
ADD_FRIEND User3 User6
ADD_FRIEND User3 User7
ADD_FRIEND User4 User8
ADD_FRIEND User5 User9
ADD_FRIEND User6 User10
SUGGEST_FRIENDS User1 5
DEGREES_OF_SEPARATION User1 User10
ADD_POST User1 "Post 1"
ADD_POST User1 "Post 2"
ADD_POST User1 "Post 3"
ADD_POST User1 "Post 4"
ADD_POST User1 "Post 5"
OUTPUT_POSTS User1 3
```

**Expected Behavior:**
- Network with 10 users and multiple friendship levels
- Friend suggestions ranked by mutual connections
- Shortest path computation across multiple hops
- Multiple posts stored and retrieved correctly

---

## 11. Implementation Notes

**Thread Safety:**  
The current implementation is single-threaded and not thread-safe. Concurrent modifications would require synchronization mechanisms.

**Scalability Considerations:**  
- Hash map load factor affects O(1) guarantees
- AVL tree remains balanced with O(log P) height
- BFS performance degrades in dense graphs (E approaches V²)

---

## 12. Troubleshooting

**Compilation errors:**
- Ensure C++11 or later (`-std=c++11` flag)
- Check that all `.hpp` files are in the same directory

**Runtime errors:**
- Verify input file format (one command per line)
- Check for proper whitespace in commands
- Ensure usernames don't contain special characters
