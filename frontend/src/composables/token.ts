import { useCommonStore } from '@/stores/common'
import { storeToRefs } from 'pinia'
import { type Ref } from 'vue'

export function useToken(): Ref<string> {
  const commonStore = useCommonStore()
  return storeToRefs(commonStore).token
}

export function setToken(token: string) {
  const commonStore = useCommonStore()
  const { setToken } = commonStore
  setToken(token)
}