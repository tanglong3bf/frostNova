import { useCommonStore } from '@/stores/common'
import axios, { type AxiosResponse } from 'axios'
import { storeToRefs } from 'pinia'

interface CustomResponse {
  code?: number
  message?: string
  data?: any
}

type NullableCustomResponse = CustomResponse | null

const instance = axios.create({
  baseURL: 'http://localhost:8000',
  timeout: 5000
})

instance.interceptors.request.use(
  config => {
    const commonStore = useCommonStore()
    const { token } = storeToRefs(commonStore)
    if (token.value !== '') {
      // 每次请求时添加 token
      config.headers['Authorization'] = 'Bearer '+ token.value
    }
    return config
  },
  error => {
    return Promise.reject(error)
  }
)

instance.interceptors.response.use(
  (response: AxiosResponse<NullableCustomResponse>) => {
    if (response.status === 204) {
      return null
    }
    const { code, message, data } = response.data || {}
    // 如果 code 小于 0，则表示发生了错误
    if (code !== undefined && code < 0) {
      console.log('code: ', code)
      return Promise.reject(new Error(message || 'Error'))
    } else {
      console.log('message: ', message || 'Success')
      return data || null
    }
  },
  error => {
    return Promise.reject(error)
  }
)

export default instance