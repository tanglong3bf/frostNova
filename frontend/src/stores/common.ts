import { defineStore } from 'pinia'
import { ref } from 'vue'

export const useCommonStore = defineStore('common', () => {
  const token = ref('')
  
  function setToken(newToken: string) {
    token.value = newToken
  }
  
  return {
    token,
    setToken
  }
}, {
  persist: true
})