# Ford-Johnson 알고리즘: 야콥스탈 수열 삽입 가이드

## 1. 야콥스탈 수열이란?

숫자 규칙:
- 0번째: 0
- 1번째: 1
- 다음 숫자 = 바로 앞 숫자 + (그 앞 숫자 * 2)

수열: **0, 1, 1, 3, 5, 11, 21, 43, 85, 171, ...**

여기서 우리가 쓸 핵심 기준 숫자는 **3, 5, 11, 21, 43, 85...** 입니다.

---

## 2. 작은 숫자들(b)을 넣는 순서

작은 숫자(b)가 총 8개 (b0 ~ b7) 있다고 가정할 때의 삽입 순서:

1. **b0** : 비교도 안 하고 그냥 맨 앞에 넣습니다.
2. **Group 1 (기준이 3번째원소)** : **b2 -> b1** 순서로 넣습니다.
3. **Group 2 (기준이 5번째원소)** : **b4 -> b3** 순서로 넣습니다.
4. **Group 3 (기준이 11번째원소)** : 원래는 11부터 거꾸로지만 8개뿐이므로 **b7 -> b6 -> b5** 순서로 넣습니다.

> **최종 넣는 순서 요약:**
> b0 -> (b2 -> b1) -> (b4 -> b3) -> (b7 -> b6 -> b5)

---

## 3. 탐색 범위 제한 (짝꿍 앞까지만 찾기)

예를 들어 **b3**을 넣을 때:
- b3은 원래 a3보다 작습니다 (b3 < a3).
- 따라서 배열 전체를 찾을 필요 없이 **a3이 있는 위치 앞까지만** 이진 탐색(`lower_bound`)을 돌립니다.

---

## 4. 실제 코드 흐름 (간단 버전)

```cpp
// 1. b1은 무조건 맨 앞에 넣기
res.insert(res.begin(), storedSmallNumPairs[0]);

// 2. 야콥스탈 순서 인덱스들 (예: 2, 1, 4, 3, 7, 6, 5...)
//    (0번 인덱스는 b1이므로 제외)
std::vector<size_t> order = {2, 1, 4, 3, 7, 6, 5}; 

for (size_t i = 0; i < order.size(); i++)
{
    size_t idx = order[i]; // 이번에 넣을 인덱스
    int smallVal = storedSmallNumPairs[idx]; // 이번 작은 값 (b)
    int bigVal = sortedBigNumbers[idx];     // 짝꿍 큰 값 (a)

    // res 안에서 내 짝꿍(bigVal)의 위치 찾기
    auto limit = std::find(res.begin(), res.end(), bigVal);

    // res 시작부터 짝꿍 앞까지만 이진 탐색!
    auto pos = std::lower_bound(res.begin(), limit, smallVal);
    res.insert(pos, smallVal);
}

// 3. 홀수라서 혼자 남았던 sadSingle이 있다면 맨 마지막에 이진 탐색으로 넣기
if (sadSingle.has_value())
{
    auto pos = std::lower_bound(res.begin(), res.end(), sadSingle.value());
    res.insert(pos, sadSingle.value());
}
```
