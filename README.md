# CENG444 - Fall 2024  
**PROGRAMMING ASSIGNMENT 1**  

---

You are required to solve the **maximum consecutive sum problem (MCS)**. The problem is defined as the following:  
Let `x[ ]` be an array of integers. For a given `x`, we want to find a subarray `x[i..j]` of `x` with maximum sum of integers.  
The problem is known as the **maximum sum subarray problem**, where the task is to find a contiguous subarray with the largest sum, within a given one-dimensional array.

---

### You are required to include the following steps in your **parallel implementation**:

1. **Calculate `s`**, as the **prefix-sums** of `x`.  
2. **Calculate `m`**, as the **prefix-mins** of `s`.  
3. **Calculate `sm`**, where `sm[j] = s[j] – m[j-1]`.  
4. **Calculate `indx`**, where `indx[j] = max{i: 0 <= i <= j and s[i] = m[j]}`

---

The **sum of numbers (MCS)** will be `max(sm(j))` and the indices will be:  
`[max(indx) + 1, minimum index of sm array with the element having the value MCS]`.

You can see the example steps as follows:  
> where the MCS value is `max(sm(j)) = 15`, the indices are `[4, 7]`.

---

For the original problem definition and the example, you can refer to the **Section V.A** of the **Peachy Parallel Assignments (EduHPC 2019)** document.

---

### Implementation Requirements

You are required to implement a **parallel version** of this problem on **OpenMP**.

---

### Notes

- You are required to run your programs with **large arrays** (may be generated randomly) and various number of threads (**2–16**), and **measure the execution time**.

---

### References

- [Maximum Subarray Problem – Wikipedia](https://en.wikipedia.org/wiki/Maximum_subarray_problem)  
- [Peachy Parallel Assignments – EduHPC 2019](https://tcpp.cs.gsu.edu/curriculum/?q=system/files/Peachy_Eduhpc_19.pdf)
